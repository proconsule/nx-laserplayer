#include "svcdnav.h"
#include <dirent.h>
#include <sys/stat.h>

void CSVCDNAV::SVCDInfo(){
    std::string dir_path = devicepath + std::string("MPEG2/");
    DIR* dir = opendir(dir_path.c_str());
    printf("CCCCCC: %s\r\n",dir_path.c_str());
    int titleidx = 0;
    if (dir != nullptr) {
        dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            const std::string file_name = entry->d_name;
            TitleInfo tmptitle;
            // Ignora le directory speciali "." e ".."
            if (file_name != "." && file_name != "..") {
                std::string _fullpath = devicepath + std::string("MPEG2/") + file_name;
                struct stat file_stat;
                if (stat(_fullpath.c_str(), &file_stat) == 0) {
                    tmptitle.filesize = file_stat.st_size;
                }
                tmptitle.path = _fullpath;
                tmptitle.titlenum = titleidx;
                titles_info.push_back(tmptitle);
                titleidx++;
            }
        }
        closedir(dir);
    }
}

void CSVCDNAV::VCDInfo(){
    std::string dir_path = devicepath + std::string("MPEGAV/");
    DIR* dir = opendir(dir_path.c_str());
    printf("CCCCCC: %s\r\n",dir_path.c_str());
    int titleidx = 0;
    if (dir != nullptr) {
        dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            const std::string file_name = entry->d_name;
            TitleInfo tmptitle;
            // Ignora le directory speciali "." e ".."
            if (file_name != "." && file_name != "..") {
                std::string _fullpath = devicepath + std::string("MPEGAV/") + file_name;
                struct stat file_stat;
                if (stat(_fullpath.c_str(), &file_stat) == 0) {
                    tmptitle.filesize = file_stat.st_size;
                }
                tmptitle.path = _fullpath;
                tmptitle.titlenum = titleidx;
                titles_info.push_back(tmptitle);
                titleidx++;
            }
        }
        closedir(dir);
    }
}