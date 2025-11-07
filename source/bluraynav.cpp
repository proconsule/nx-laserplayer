/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "bluraynav.h"

#include <libbluray/bluray.h>
#include <libbluray/log_control.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CBLURAYNAV::~CBLURAYNAV(){
    
}

static void _indx_print_title(const BLURAY_TITLE *title, int normal_title)
{
    printf("    object type   : %s\n", title->bdj ? "BD-J" : "HDMV");
    printf("    playback type : %s\n", title->interactive ? "Interactive" : "Movie");
    printf(title->bdj ?
           "    name          : %05d.bdjo\n" :
           "    id_ref        : %u\n",
           title->id_ref);

    if (normal_title) {
        printf("    access type   : %s%s\n", title->accessible ? "Accessible" : "Prohibited", title->hidden ? ", Hidden" : "");
    }
}

static void _indx_print(const BLURAY_DISC_INFO *info)
{
    uint32_t i;

    printf("\nFirst playback:\n");
    if (info->first_play) {
        _indx_print_title(info->first_play, 0);
    } else {
        printf("    (not present)\n");
    }

    printf("\nTop menu:\n");
    if (info->top_menu) {
        _indx_print_title(info->titles[0], 0);
    } else {
        printf("    (not present)\n");
    }

    printf("\nTitles: %d\n", info->num_titles);
    for (i = 1; i <= info->num_titles; i++) {
        printf("%02d %s\n", i, info->titles[i]->name ? info->titles[i]->name : "");
        _indx_print_title(info->titles[i], 1);
    }
}

void CBLURAYNAV::BDInfo(){
    BLURAY * test = bd_open(devicepath.c_str(),NULL);
    //bd_set_debug_mask(DBG_AACS);
    int titlenums =  bd_get_titles(test, TITLES_RELEVANT, 0);
     
     
    
    for(int i=0;i<titlenums;i++){
        BLURAY_TITLE_INFO *testtitle =  bd_get_title_info(test, i, 0);
        TitleInfo tmptitle;
        tmptitle.titlenum = i+1;
        uint64_t mytime = 0;
        for(int j = 0;j<testtitle->chapter_count;j++){
            //printf("CHAP: %d %lu %lu %lu\r\n",testtitle->chapters[j].idx,testtitle->chapters[j].start/90000,testtitle->chapters[j].duration/90000,testtitle->chapters[j].offset);
           TitleInfo::Chapter tmpchapter;
           tmpchapter.num = i;
           tmpchapter.duration = testtitle->chapters[j].duration/90000;
           tmpchapter.start = testtitle->chapters[j].start/90000;
           tmpchapter.end = tmpchapter.start +tmpchapter.duration;
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
    bd_close(test);
}
