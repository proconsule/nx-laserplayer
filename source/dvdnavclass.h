#ifndef DVDNAVCLASS_H
#define DVDNAVCLASS_H

#include <string>
#include <vector>
#include <cstring>
#include "discinfo_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <dvdnav/dvdnav.h>
#include <dvdread/ifo_read.h>


#ifdef __cplusplus
}
#endif



class CDVDNAV : public CDISCINFO{
public:
    CDVDNAV(std::string _path) : CDISCINFO(_path){};
    ~CDVDNAV();
    void GetDVDInfo();
    void GetDVDMenu();
    void get_chapter_durations_robust(dvd_reader_t *dvdread , int title_number);
    //std::vector<titles_info_struct> titles;
    
    
    int dvd_time_to_seconds(dvd_time_t *dt);
    int find_existing_vts_for_title(dvd_reader_t *dvd, int title_number, ifo_handle_t *vmg_ifo);
    void parseTitle(int titleidx);
    
    
private:
    //std::string devicepath;
    dvdnav_t* dvd;
    dvd_reader_t *dvdread;
    
    
};

#endif /* DVDNAVCLASS_H */