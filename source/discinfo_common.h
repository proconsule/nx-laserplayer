#ifndef DISCINFO_COMMON_H
#define DISCINFO_COMMON_H

#include <string>
#include <vector>
#include <cstring>
#include <cstdint>

typedef struct{
    uint32_t num;
    uint32_t start;
    uint32_t end;
    uint64_t duration;
    
}parts_info_struct;

typedef struct {
    char lang[16];
    char format[16];
    int stream_id; 
}audio_tracks_struct;

typedef struct {
    char lang[8];           
    char type[32];          
    char coding_mode[16];   
    int stream_id;         
} subtitle_tracks_struct;

typedef struct{
    uint32_t titlenum = 0;
    uint64_t titletime = 0;
    std::vector<parts_info_struct> parts;
    std::vector<audio_tracks_struct> audio_tracks;
    std::vector<subtitle_tracks_struct> sub_tracks;
}titles_info_struct;


class TitleInfo {

public:

    class Track {
    public:
        int id;
        std::string type;
        std::string title = "Unknown";
        std::string language = "N/A";
        std::string codec;
        int channels;
        int bit_rate;
        int sample_rate;
        int width;
        int height;
        bool albumart = false;
        bool selected = false;
    };
	
    class Chapter{
	public:
        uint32_t num;
        uint32_t start;
        uint32_t end;
        uint64_t duration;
    };

    int titlenum = 0;
    int titletime = 0;
    std::string title = "Unknown";
    std::string path;
    long duration = 0;
    long resume = 0;
    int bit_rate = 0;
    bool interlaced = false;
    std::vector<Track> videos;
    std::vector<Track> audios;
    std::vector<Track> subtitles;
	
    std::vector<Chapter> chapters;


};


class CDISCINFO{
public:
    CDISCINFO(std::string _path);
    //~CDISCINFO();
    
    
    std::vector<TitleInfo> titles_info;
    void updateTitleTracksList(int titlenum,std::vector<TitleInfo::Track> _rawtracks);
    int GetChapterIDX(uint32_t _titlenum,uint32_t _time);
    
    
protected:
    std::string devicepath;
   
    
    
};





#endif /* DISCINFO_COMMON_H */

