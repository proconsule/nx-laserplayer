#include "iniparser.h"


CIniParser::CIniParser(std::string inifile){
    ini_path = inifile;
    ReadConfig();
	
}

CIniParser::~CIniParser(){
	
	
}

void CIniParser::ReadConfig(){
    
        mINI::INIFile *fileref;
	mINI::INIStructure inidata;
	
        fileref = new mINI::INIFile(ini_path);
	
	inidata.clear();
	
	bool opened =  fileref->read(inidata);
	
	if(opened){
	
		if(inidata["Main"].has("audio_lang")){
			nx_laserplayer_config.audio_lang = inidata.get("Main").get("audio_lang");
		}
		if(inidata["Main"].has("sub_lang")){
			nx_laserplayer_config.sub_lang = inidata.get("Main").get("sub_lang");
		}	
	}
        
        
        nx_laserplayer_config_tmp = nx_laserplayer_config;
	
}

void CIniParser::RevertChanges(){
    nx_laserplayer_config_tmp = nx_laserplayer_config;
}

void CIniParser::saveSettings(nx_laserplayer_config_struct *_savecfg){
    
    mINI::INIFile *fileref;
    mINI::INIStructure inidata;
    fileref = new mINI::INIFile(ini_path);
    bool opened =  fileref->read(inidata);
    
    inidata["Main"].set({
            {"audio_lang", _savecfg->audio_lang},
            {"sub_lang", _savecfg->sub_lang}
        });
    
    
    if(opened){
        bool done = fileref->write(inidata, true);
	}else{
        bool done = fileref->generate(inidata, true);
    }
        
    
    
}