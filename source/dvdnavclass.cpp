#include "dvdnavclass.h"
#include <iomanip>
#include <algorithm>


int CDVDNAV::dvd_time_to_seconds(dvd_time_t *dt) {
    int seconds = 0;
    seconds += ((dt->hour >> 4) * 10 + (dt->hour & 0x0f)) * 3600;
    seconds += ((dt->minute >> 4) * 10 + (dt->minute & 0x0f)) * 60;
    seconds += (dt->second >> 4) * 10 + (dt->second & 0x0f);
    return seconds;
}
int CDVDNAV::find_existing_vts_for_title(dvd_reader_t *dvd, int title_number, ifo_handle_t *vmg_ifo) {
    title_info_t *title_info = &vmg_ifo->tt_srpt->title[title_number - 1];
    int indicated_vts = title_info->title_set_nr;
    ifo_handle_t *test_ifo = ifoOpen(dvd, indicated_vts);
    if (test_ifo) {
        ifoClose(test_ifo);
        return indicated_vts;
    }
    for (int vts = 1; vts <= 9; vts++) {
        test_ifo = ifoOpen(dvd, vts);
        if (test_ifo) {
            
            if (test_ifo->vts_ptt_srpt && 
                test_ifo->vts_ptt_srpt->nr_of_srpts >= title_info->vts_ttn) {
                ifoClose(test_ifo);
                return vts;
            }
            ifoClose(test_ifo);
        }
    }
    return 1; 
}


std::string createVobPath(int titleNumber, int vobId) {
    std::string path;
    std::stringstream ss;

    // Formatta il numero del titolo (es. 1 -> 01)
    ss << "VTS_";
    ss << std::setw(2) << std::setfill('0') << titleNumber;
    ss << "_";

    // Formatta il numero del VOB ID (es. 1 -> 1)
    ss << vobId;
    ss << ".VOB";

    path = ss.str();
    return path;
}

void CDVDNAV::get_chapter_durations_robust(dvd_reader_t *dvdread, int title_number) {
    if (!dvdread) return;
    ifo_handle_t *vmg_ifo = ifoOpen(dvdread, 0);
    if (!vmg_ifo) {
        ifoClose(vmg_ifo);
        DVDClose(dvdread);
        return;
    }
   
    tt_srpt_t *tt_srpt = vmg_ifo->tt_srpt;
    if (title_number > tt_srpt->nr_of_srpts) {
        ifoClose(vmg_ifo);
        DVDClose(dvdread);
        return;
    }
    title_info_t *title_info = &tt_srpt->title[title_number - 1];
    int ttn = title_info->vts_ttn;
    
    int vts_number = find_existing_vts_for_title(dvdread, title_number, vmg_ifo);
    ifo_handle_t *vts_ifo = ifoOpen(dvdread, vts_number);
    
    if (!vts_ifo) {
        ifoClose(vmg_ifo);
        DVDClose(dvdread);
        return;
    }
    
    if (!vts_ifo->vts_ptt_srpt || ttn > vts_ifo->vts_ptt_srpt->nr_of_srpts) {
        ttn = 1;
    }
    
    vts_ptt_srpt_t *vts_ptt_srpt = vts_ifo->vts_ptt_srpt;
    ptt_info_t *ptt_info = &vts_ptt_srpt->title[ttn - 1].ptt[0];
    int pgc_id = ptt_info->pgcn - 1;
    pgc_t *pgc = vts_ifo->vts_pgcit->pgci_srp[pgc_id].pgc;
    
    if(pgc){
        for (int i = 0; i < pgc->nr_of_programs; i++) {
    
            dvd_time_t *chapter_time = &pgc->cell_playback[i].playback_time;
            TitleInfo::Chapter tmpchapter;
            //parts_info_struct tmp;
            if(i==0){
                tmpchapter.start = 0;
                tmpchapter.end = dvd_time_to_seconds(chapter_time);
            }else{
                tmpchapter.start = titles_info[title_number - 1].chapters[i-1].end;
                tmpchapter.end = tmpchapter.start+dvd_time_to_seconds(chapter_time);
            }
            tmpchapter.num = i+1;
            tmpchapter.duration = dvd_time_to_seconds(chapter_time);
            titles_info[title_number - 1].chapters.push_back(tmpchapter);
        }
    }
    ifoClose(vts_ifo);
    ifoClose(vmg_ifo);
}


void CDVDNAV::parseTitle(int titleidx){
    
    
    if (dvdnav_title_play(dvd, titleidx) != DVDNAV_STATUS_OK) {
        printf("Errore: titolo %d non trovato\n", titleidx);
        dvdnav_close(dvd);
        return;
    }
   /*
   
    uint8_t logical_stream, physical_stream;
    int num_audio_streams = 0;
    for (int j = 0; j < 8; j++) {
        if (dvdnav_get_audio_logical_stream(dvd, j) == DVDNAV_STATUS_OK) {
            num_audio_streams++;
        }
    }
    printf("NUMERO DI STREAM AUDIO : %d\r\n",num_audio_streams);
   */
}


void CDVDNAV::GetDVDInfo(){
    dvdnav_t* dvd;
    const char* device_path = devicepath.c_str();
    int err;
    err = dvdnav_open(&dvd, device_path);
    dvdnav_set_readahead_flag(dvd, 1);
    if(err == DVDNAV_STATUS_ERR ){
        return;
    }
    dvdread = DVDOpen(device_path);
    if (!dvdread) return;
   
    
    int numero_titoli;
    err = dvdnav_get_number_of_titles(dvd, &numero_titoli);
    if (err < 0) {
        dvdnav_close(dvd);
        return ;
    }
    for(int i=1;i<=numero_titoli;i++){
        titles_info_struct tmp;
        
        TitleInfo tmptitle;
        
        tmptitle.titlenum = i;
        
        titles_info.push_back(tmptitle);
        //titles.push_back(tmp);
        get_chapter_durations_robust(dvdread,i);
        //parseTitle(i);

        
        uint64_t mytime = 0;
        for(int j=0;j<titles_info[i-1].chapters.size();j++){
            mytime+=titles_info[i-1].chapters[j].duration;
        }
        titles_info[i-1].titletime = mytime;
        
    }
    
        DVDClose(dvdread);
    
        dvdnav_close(dvd);
    
    
}

void CDVDNAV::GetDVDMenu(){
    dvdnav_t* dvd;
    const char* device_path = devicepath.c_str();
    int err;
    err = dvdnav_open(&dvd, device_path);
    if(err == DVDNAV_STATUS_ERR ){
        return;
    }
    dvdread = DVDOpen(device_path);
    if (!dvdread) return;
    ifo_handle_t *vmg_ifo = ifoOpen(dvdread, 0);
    if (!vmg_ifo) {
        DVDClose(dvdread);
        return;
    }
    if (vmg_ifo) {
        
        // Informazioni sui menu del VMG
        printf("Numero di menu VMG: %d\n", vmg_ifo->pgci_ut->nr_of_lus);

        
        for (int i = 0; i < vmg_ifo->pgci_ut->nr_of_lus; i++) {
            pgci_lu_t *lu = &vmg_ifo->pgci_ut->lu[i];
            printf("Menu %d: %d programmi\n", i, lu->pgcit->nr_of_pgci_srp);
            
            for (int j = 0; j < lu->pgcit->nr_of_pgci_srp; j++) {
                pgc_t *pgc = lu->pgcit->pgci_srp[j].pgc;
                printf("  Programma %d: %d celle\n", j, pgc->nr_of_cells);
                printf("VOB ID %d\r\n",pgc->cell_position->vob_id_nr);
                printf("CELL POSITION OFFSET %d\r\n",pgc->cell_position_offset);
                
                
            }
        }

        ifoClose(vmg_ifo);
    
    }
    fflush(stdout);
    
    DVDClose(dvdread);
    
    dvdnav_close(dvd);
}




CDVDNAV::~CDVDNAV(){
   
}