#include <iomanip>
#include <sstream>
#include "libmpv.h"
#include <stdio.h>
//#include "utils.h"



libMpv::libMpv(const std::string &configDir) {

    handle = mpv_create();
    if (!handle) {
        NXLOG::ERRORLOG("MPV Error Create Handle\n");
        return;
    }
    
    NXLOG::DEBUGLOG("MPV Handle Created\n");
    mpv_set_option_string(handle, "config", "yes");
    mpv_set_option_string(handle, "config-dir", configDir.c_str());
    mpv_set_option_string(handle, "terminal", "yes");
    //if(NXLOG::loglevel == 0){
        mpv_set_option_string(handle, "msg-level", "all=no");
    //}else{
    //  mpv_set_option_string(handle, "msg-level", "all=v,vd=trace,vo=trace");
    //}
    mpv_set_option_string(handle, "vd-lavc-threads", "4");
    //mpv_set_option_string(handle, "vd-lavc-skiploopfilter", "all");
    mpv_set_option_string(handle, "audio-channels", "stereo");
    mpv_set_option_string(handle, "video-timing-offset", "0");
    mpv_set_option_string(handle, "osd-bar-align-y", "0.9");
    //mpv_set_option_string(handle, "fbo-format", "rgb10_a2");
    mpv_set_option_string(handle, "volume-max", "200");
    //mpv_set_option_string(handle, "vd-lavc-dr", "yes");
    //default Font Style
    mpv_set_option_string(handle, "sub-border-size", "3");
    mpv_set_option_string(handle, "sub-shadow-offset", "1");
    mpv_set_option_string(handle, "sub-shadow-color", "0.0/0.0/0.0/0.25");
    
    mpv_set_option_string(handle, "reset-on-next-file", "pause");
    
    mpv_set_option_string(handle, "interpolation", "no");
    mpv_set_option_string(handle, "scale", "linear");
    mpv_set_option_string(handle, "user-agent","Mozilla/5.0 (X11; Linux x86_64; rv:49.0) Gecko/20100101 Firefox/49.0");

    
    mpv_set_option_string(handle, "image-display-duration", "inf");

    mpv_set_option_string(handle, "hdr-compute-peak", "yes");
    mpv_set_option_string(handle, "demuxer-seekable-cache", "yes");
    
    
    mpv_command_string(handle, "set sub-auto external");      
    mpv_command_string(handle, "set sub-font-size 45");       
    mpv_command_string(handle, "set sub-color '#FFFFFF'");  
    mpv_command_string(handle, "set sub-border-color '#000000'");
    mpv_command_string(handle, "set sub-border-size 2");    
    
    
    mpv_command_string(handle, "set sub-ass-override no");
    mpv_set_option_string(handle, "hwdec", "nvtegra");
    mpv_set_option_string(handle, "hwdec-codecs", "mpeg1video,mpeg2video,mpeg4,vc1,wmv3,h264,hevc,vp8,vp9,mjpeg");
    mpv_set_option_string(handle, "ao", "hos");
    
    
    
    NXLOG::DEBUGLOG("MPV Handle initialize\n");
    int res = mpv_initialize(handle);
    if (res) {
        NXLOG::ERRORLOG("libmpv_initialize: %s\n", mpv_error_string(res));
        mpv_terminate_destroy(handle);
        return;
    }
    
    NXLOG::DEBUGLOG("MPV Init Renderer\n");
     
    int advanced_control = 1;
    
    mpv_version = mpv_get_property_string(handle, "mpv-version");
    ffmpeg_version = mpv_get_property_string(handle, "ffmpeg-version");
        
    
    
    NXLOG::DEBUGLOG("MPV Init Completed\n");
    
}

void libMpv::loadBluRay(std::string _path){
    BLURAYNAV = new CBLURAYNAV(_path);
    BLURAYNAV->BDInfo();
    
    int _testduration = 0;
    for(int i=0;i<BLURAYNAV->titles_info.size();i++){
        if(BLURAYNAV->titles_info[i].titletime>_testduration){
            _playidx = i;
            _testduration = BLURAYNAV->titles_info[i].titletime;
        }
        
        for(int j =0 ;j<BLURAYNAV->titles_info[i].chapters.size();j++){
            printf("\tChapter %d start: %d end: %d duration: %d\r\n",BLURAYNAV->titles_info[i].chapters[j].num,BLURAYNAV->titles_info[i].chapters[j].start,BLURAYNAV->titles_info[i].chapters[j].end,BLURAYNAV->titles_info[i].chapters[j].duration);
        }
    }
    
    mpv_observe_property(handle, 0, "time-pos", MPV_FORMAT_INT64);
    mpv_observe_property(handle, 1, "duration", MPV_FORMAT_INT64);
    mpv_observe_property(handle, 2, "pause", MPV_FORMAT_FLAG);
    mpv_observe_property(handle, 3, "aid", MPV_FORMAT_INT64);
    mpv_observe_property(handle, 4, "sid", MPV_FORMAT_INT64);
    
    std::string opencommand = "loadfile bluray://"+ std::to_string(_playidx);
    mpv_set_option_string(handle, "bluray-device", _path.c_str());
    mpv_command_string(handle, opencommand.c_str());
    
    
}

void libMpv::loadSVCD(std::string _path){
    mpv_observe_property(handle, 0, "time-pos", MPV_FORMAT_INT64);
    mpv_observe_property(handle, 1, "duration", MPV_FORMAT_INT64);
    mpv_observe_property(handle, 2, "pause", MPV_FORMAT_FLAG);
    std::string opencommand = "loadfile " + _path + std::string("MPEG2/");
    mpv_set_option_string(handle, "dvd-device", _path.c_str());
    mpv_command_string(handle, opencommand.c_str());
}

void libMpv::loadVCD(std::string _path){
    mpv_observe_property(handle, 0, "time-pos", MPV_FORMAT_INT64);
    mpv_observe_property(handle, 1, "duration", MPV_FORMAT_INT64);
    mpv_observe_property(handle, 2, "pause", MPV_FORMAT_FLAG);
    std::string opencommand = "loadfile " + _path + std::string("MPEGAV/");
    mpv_set_option_string(handle, "dvd-device","");
    mpv_command_string(handle, opencommand.c_str());
}

void libMpv::loadDVD(std::string _path){
    if(DVDNAV == nullptr){
        DVDNAV = new CDVDNAV(_path);
    }
    
    DVDNAV->GetDVDInfo();
    
    int _testduration = 0;
    for(int i=0;i<DVDNAV->titles_info.size();i++){
        printf("Title: %u Duration: %u Parts: %u\r\n",DVDNAV->titles_info[i].titlenum,DVDNAV->titles_info[i].titletime,DVDNAV->titles_info[i].chapters.size());
        if(DVDNAV->titles_info[i].titletime>_testduration){
            _playidx = i;
            _testduration = DVDNAV->titles_info[i].titletime;
        }
        
        for(int j =0 ;j<DVDNAV->titles_info[i].chapters.size();j++){
            printf("\tChapter %d start: %d end: %d duration: %d\r\n",DVDNAV->titles_info[i].chapters[j].num,DVDNAV->titles_info[i].chapters[j].start,DVDNAV->titles_info[i].chapters[j].end,DVDNAV->titles_info[i].chapters[j].duration);
        }
    }
    
    if(_playidx >=0){
        std::string opencommand = "loadfile dvd://" + std::to_string(_playidx);
        mpv_set_option_string(handle, "dvd-device", _path.c_str());
        mpv_command_string(handle, opencommand.c_str());

        mpv_observe_property(handle, 0, "time-pos", MPV_FORMAT_INT64);
        mpv_observe_property(handle, 1, "duration", MPV_FORMAT_INT64);
        mpv_observe_property(handle, 2, "pause", MPV_FORMAT_FLAG);
        mpv_observe_property(handle, 3, "aid", MPV_FORMAT_INT64);
        mpv_observe_property(handle, 4, "sid", MPV_FORMAT_INT64);
        
        
    }
   
}


void libMpv::parseTracksInfo(mpv_node *node){
std::vector<TitleInfo::Track> rawtracklist;


if (node->format == MPV_FORMAT_NODE_ARRAY) {
    for (int i = 0; i < node->u.list->num; i++) {
        if (node->u.list->values[i].format == MPV_FORMAT_NODE_MAP) {
            TitleInfo::Track tmptrack;
            for (int n = 0; n < node->u.list->values[i].u.list->num; n++) {
                std::string key = node->u.list->values[i].u.list->keys[n];
                printf("KEY: %s\r\n",key.c_str());
                if (key == "type") {
                    if (node->u.list->values[i].u.list->values[n].format == MPV_FORMAT_STRING) {
                        tmptrack.type = node->u.list->values[i].u.list->values[n].u.string;
                    }
                }
                if (key == "id") {
                    if (node->u.list->values[i].u.list->values[n].format == MPV_FORMAT_INT64) {
                        tmptrack.id = (int) node->u.list->values[i].u.list->values[n].u.int64;
                    }
                }
                if (key == "lang") {
                    if (node->u.list->values[i].u.list->values[n].format == MPV_FORMAT_STRING) {
                        tmptrack.language = node->u.list->values[i].u.list->values[n].u.string;
                    }
                }
                if (key == "codec") {
                    if (node->u.list->values[i].u.list->values[n].format == MPV_FORMAT_STRING) {
                        tmptrack.codec = node->u.list->values[i].u.list->values[n].u.string;
                    }
                }
                if (key == "selected") {
                    tmptrack.selected = (bool)node->u.list->values[i].u.list->values[n].u.flag;
                }
                
            }
            rawtracklist.push_back(tmptrack);
            //printf("Track id: %d %s %s\r\n",tmptrack.id,tmptrack.type.c_str(),tmptrack.language.c_str());
        }
        
    }
    
}

if(DVDNAV!=nullptr && this->_playidx >= 0 ){
    DVDNAV->updateTitleTracksList(this->_playidx,rawtracklist);
}
if(BLURAYNAV!=nullptr && this->_playidx >= 0 ){
    BLURAYNAV->updateTitleTracksList(this->_playidx,rawtracklist);
}




}



void libMpv::loadDvdMenu(std::string _path){
    if(DVDNAV == nullptr){
        DVDNAV = new CDVDNAV(_path);
    }
    DVDNAV->GetDVDMenu();
}

void libMpv::Stop(){
    mpv_command_string(handle, "stop");
}

void libMpv::Pause() {
	mpv_command_string(handle, "set pause yes");
}

void libMpv::Resume() {
	mpv_command_string(handle, "set pause no");
}

mpv_handle *libMpv::getHandle() {
    return handle;
}

mpv_render_context *libMpv::getContext() {
    return context;
}

libMpv::~libMpv(){
    if(DVDNAV!=nullptr)delete DVDNAV;
    if(BLURAYNAV!=nullptr)delete BLURAYNAV;
    printf("AAAAAAAAAAAAAAAAAA\r\n");
    fflush(stdout);
    mpv_command_string(handle, "quit");
    while (1) {
    mpv_event *event = mpv_wait_event(handle, 1.0);
        if (event->event_id == MPV_EVENT_SHUTDOWN) {
            break;
        }
        
    }
     printf("BBBBBBBBBBBBBBBBBBB\r\n");
     fflush(stdout);
    
    if (handle) {
        mpv_terminate_destroy(handle);
         printf("CCCCCCCCCCCCCCCCCCCCCCCCCCC\r\n");
         fflush(stdout);
    }
}

void libMpv::seek(double position,bool osd) {
    position = getPosition()+position;
    if(position <=0)position = 0;
    if(osd){
        std::string cmd = "seek " + std::to_string(position) + " absolute";
        mpv_command_string(handle, cmd.c_str());
    }else{
        std::string cmd = "no-osd seek " + std::to_string(position) + " absolute";
        mpv_command_string(handle, cmd.c_str());
    }
}

int64_t libMpv::getPosition() {
    int64_t position = 0;
    mpv_get_property(handle, "playback-time", MPV_FORMAT_INT64, &position);
    return position;
}

void libMpv::set_audio_track(uint32_t _idx){
    
    std::string cmd = "set aid " + std::to_string(_idx);
    mpv_command_string(handle, cmd.c_str());
    
    
}

void libMpv::set_sub_track(uint32_t _idx){
    std::string cmd = "set sid " + std::to_string(_idx);
    mpv_command_string(handle, cmd.c_str());
}

void libMpv::seekToSecond(double seconds){
    const char* cmd[] = {"seek", std::to_string(seconds).c_str(), "absolute", nullptr};
    int error = mpv_command(handle, cmd);
}

void libMpv::setAspectRatio(const char* ratio){
    mpv_set_option_string(handle, "video-aspect-override", ratio);
    
    
}

void libMpv::setDeinterlace(bool _active){
    if(_active){
        mpv_set_property_string(handle, "deinterlace", "yes");
    }else{
         mpv_set_property_string(handle, "deinterlace", "no");
    }
    
}

void printAudioTrackInfo(mpv_node* track, int index) {
        std::string lang = "Unknown";
        std::string title = "";
        int id = -1;
        
        // Cerca nelle proprietà della traccia
        for (int j = 0; j < track->u.list->num; j++) {
            std::string key = track->u.list->keys[j];
            mpv_node* value = &track->u.list->values[j];
            
            if (key == "lang" && value->format == MPV_FORMAT_STRING) {
                lang = value->u.string;
            } else if (key == "title" && value->format == MPV_FORMAT_STRING) {
                title = value->u.string;
            } else if (key == "id" && value->format == MPV_FORMAT_INT64) {
                id = value->u.int64;
            }
        }
        
        std::cout << "Audio " << index << ": ID=" << id 
                  << ", Lang=" << lang << ", Title=" << title << std::endl;
    }

void libMpv::processEvents(){
    while (1) {
        
            mpv_event *mp_event = (mpv_event*) mpv_wait_event(handle, 0);
            if (mp_event->event_id == MPV_EVENT_NONE){
                break;
            }
            if (mp_event->event_id == MPV_EVENT_SHUTDOWN){
                break;
            }
            if (mp_event->event_id == MPV_EVENT_LOG_MESSAGE) {
                //mpv_event_log_message *msg = (mpv_event_log_message *) mp_event->data;
                break;
            }
            if (mp_event->event_id == MPV_EVENT_FILE_LOADED) {
                
                            mpv_node node;
                            int ret = mpv_get_property(handle, "track-list", MPV_FORMAT_NODE, &node);
                            parseTracksInfo(&node);
                            bool is_interlaced;
                            mpv_get_property(handle, "video-frame-info/interlaced", MPV_FORMAT_FLAG, &is_interlaced);

                            if (is_interlaced) {
                                setDeinterlace(true);
                        
                            } else {
                                setDeinterlace(false);
                               
                            }
                        
                        }
                    
            if (mp_event->event_id == MPV_EVENT_START_FILE) {

                
                disc_playback = true;
                
                        
            }
            if (mp_event->event_id == MPV_EVENT_END_FILE) {
                
                
                disc_playback = false;  
            }
            
            if(mp_event->event_id == MPV_EVENT_PROPERTY_CHANGE){
                            mpv_event_property *prop = (mpv_event_property *)mp_event->data;
                            
                if (prop->name && strcmp(prop->name, "time-pos") == 0 && prop->format == MPV_FORMAT_INT64) {
                        current_playback_time = *(int64_t*)prop->data;
                }
                            
                if (prop->name && strcmp(prop->name, "duration" ) == 0 && prop->format == MPV_FORMAT_INT64) {
                    duration_playback_time = *(int64_t*)prop->data;
                }
                
                            if(mp_event->reply_userdata == 2){
                                paused = *(bool *)prop->data;
                                printf("PAUSED: %d\r\n",paused);
                                fflush(stdout);
                                      
                            }
                            
                            
                            if (strcmp(prop->name, "aid") == 0) {
                                if (prop->format == MPV_FORMAT_INT64 && prop->data) {
                                    audioID= *(int64_t*)prop->data;
                                    
                                }
                                
                            }
                            if (strcmp(prop->name, "sid") == 0) {
                                if (prop->format == MPV_FORMAT_INT64 && prop->data) {
                                    subID= *(int64_t*)prop->data;
                                    
                                }
                                
                            }
                
            }
            
            if(mp_event->event_id!=22){
                //printf("event: %s %d\n", mpv_event_name(mp_event->event_id),mp_event->event_id);
            }
        }
}