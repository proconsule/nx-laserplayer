#ifndef INIPARSER_H
#define INIPARSER_H

#define MINI_CASE_SENSITIVE
#include "../libs/mINI/src/mini/ini.h"

typedef struct{
    std::string audio_lang;
    std::string sub_lang;
    
    
}nx_laserplayer_config_struct;


class CIniParser{
public:
	CIniParser(std::string inifile);
	~CIniParser();
        
        void ReadConfig();
        void RevertChanges();
        void saveSettings(nx_laserplayer_config_struct *_savecfg);
        
        
        nx_laserplayer_config_struct * getConfig(){
            return &nx_laserplayer_config_tmp;
        }
        
private:
    std::string ini_path;
    nx_laserplayer_config_struct nx_laserplayer_config;
    nx_laserplayer_config_struct nx_laserplayer_config_tmp;
        
};


#endif /* INIPARSER_H */

