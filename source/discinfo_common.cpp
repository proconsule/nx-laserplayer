#include "discinfo_common.h"


CDISCINFO::CDISCINFO(std::string _path){
    devicepath = _path;
}

void CDISCINFO::updateTitleTracksList(int titleidx,std::vector<TitleInfo::Track> _rawtracks){
    
    titles_info[titleidx].audios.clear();
    titles_info[titleidx].subtitles.clear();
    for(int i=0;i<_rawtracks.size();i++){
        if(_rawtracks[i].type == "audio"){
            titles_info[titleidx].audios.push_back(_rawtracks[i]);
        }
        if(_rawtracks[i].type == "sub"){
            titles_info[titleidx].subtitles.push_back(_rawtracks[i]);
        }
        
    }
    
}

int CDISCINFO::GetChapterIDX(uint32_t _titlenum,uint32_t _time){
    
    if(_titlenum >= titles_info.size())return -1;
    
    for(int i=0;i< titles_info[_titlenum].chapters.size();i++){
        if(_time<titles_info[_titlenum].chapters[i].end)return i;
    }
    
    return -1;
}