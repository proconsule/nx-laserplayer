#ifndef NXMP_LIBMPVMPV_H
#define NXMP_LIBMPVMPV_H


#include <string>
#include <mpv/client.h>
#include <deko3d.hpp>

#include <mpv/render_dk3d.h>

#include <vector>
#include <utility>

#include "dvdnavclass.h"
#include "bluraynav.h"
#include "svcdnav.h"
#include "iniparser.h"
#include "usbdvd.h"

#include "logger.h"


class libMpv{
public:

	libMpv(const std::string &configDir);
	
	~libMpv();
	
	int64_t getPosition();
    void loadDVD(std::string _path);
    void loadBluRay(std::string _path);
    void loadSVCD(std::string _path);
    void loadVCD(std::string _path);
    void loadFile(std::string _path);
	void loadDvdMenu(std::string _path);
    void loadBlurayMenu(std::string _path);
    void loadDVDTitle(std::string _path,int _titleid);
    void loadBDTitle(std::string _path,int _titleid);
	void Stop();
	void set_audio_track(uint32_t _idx);
	void set_sub_track(uint32_t _idx);
	void setAspectRatio(const char* ratio);
	void seekToSecond(double seconds);
	void seek(double position,bool osd);
    void setDeinterlace(bool _active);
    
    void Pause();
    void Resume();
	
	mpv_handle *getHandle();

	mpv_render_context *getContext();
	
	std::string mpv_version;
	std::string ffmpeg_version;
	
	CDVDNAV* DVDNAV = nullptr;
    CBLURAYNAV *BLURAYNAV = nullptr;
    CSVCDNAV *SVCDNAV = nullptr;
    
    CUSBDVD* usbdvd = nullptr;
	int _playidx = -1;
    CIniParser *iniparser;
    
    void processEvents();
    
    void parseTracksInfo(mpv_node *node);
    
    bool disc_playback = false;
    
    uint32_t current_playback_time = 0;
	uint32_t duration_playback_time = 0;
    
    bool paused = false;
    
    uint32_t audioID = -1;
    uint32_t subID = -1;
	
private:


	
	mpv_handle *handle = nullptr;
	mpv_render_context *context = nullptr;
    
    
	
	
	
};

#endif
