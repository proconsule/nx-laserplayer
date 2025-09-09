/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "bluraynav.h"

CBLURAYNAV::~CBLURAYNAV(){
    
}

void CBLURAYNAV::BDInfo(){
    BLURAY * test = bd_open(devicepath.c_str(),NULL);
    int titlenums =  bd_get_titles(test, TITLES_RELEVANT, 0);
    
    for(int i=0;i<titlenums;i++){
        BLURAY_TITLE_INFO *testtitle =  bd_get_title_info(test, i, 0);
        //titles_info_struct tmp;
        TitleInfo tmptitle;
        tmptitle.titlenum = i+1;
        //get_chapter_durations_robust(dvdread,i);
        uint64_t mytime = 0;
        for(int j = 0;j<testtitle->chapter_count;j++){
            //printf("CHAP: %d %lu %lu %lu\r\n",testtitle->chapters[j].idx,testtitle->chapters[j].start/90000,testtitle->chapters[j].duration/90000,testtitle->chapters[j].offset);
           TitleInfo::Chapter tmpchapter;
           tmpchapter.num = i;
           tmpchapter.duration = testtitle->chapters[j].duration/90000;
           tmpchapter.start = testtitle->chapters[j].start/90000;
           tmpchapter.end = tmpchapter.start +tmpchapter.duration;
           parts_info_struct tmppart;
           
           tmptitle.chapters.push_back(tmpchapter);
        }
        
        for(int j=0;j<tmptitle.chapters.size();j++){
            mytime+=tmptitle.chapters[j].duration;
        }
        tmptitle.titletime = mytime;
        titles_info.push_back(tmptitle);
        
    }
    
    printf("TITLES: %d\r\n",titlenums);
    for(int i=0;i<titlenums;i++){
        BLURAY_TITLE_INFO *testtitle =  bd_get_title_info(test, i, 0);
        
        printf("NUM Chapter: %d\r\n",testtitle->chapter_count);
        for(int j = 0;j<testtitle->clips->audio_stream_count;j++){
            printf("AUDIO: %s\r\n",testtitle->clips->audio_streams[j].lang);
        }
        for(int j = 0;j<testtitle->chapter_count;j++){
            printf("CHAP: %d %lu %lu %lu\r\n",testtitle->chapters[j].idx,testtitle->chapters[j].start/90000,testtitle->chapters[j].duration/90000,testtitle->chapters[j].offset);
           
        }
    }
}
