#ifndef NXMP_GUI_H
#define NXMP_GUI_H

#include <vector>
#include <string>


#include <switch.h>
#include <libmpv.h>
#include <nxmp-render.h>
#include <usbdvd.h>
#include "imgloader.h"
#include "utils.h"

class CGUI{
public:
	CGUI(uint32_t _w,uint32_t _h);
	
	void RenderLoop();
	
	void HandleEvents();
	void HandleLayers();
	void HandleRender();
	
	NXMPRenderer *Renderer = nullptr;
	libMpv *libmpv = nullptr;
	CUSBDVD *usbdvd = nullptr;
	CImgLoader *imgloader = nullptr;
	
	void CheckDiscType();
private:
	
	uint64_t Process_UI_Events(std::chrono::time_point<std::chrono::system_clock> myeventtime);
	
	void PlayerGUI();
	void MainMenuGUI();
	void AudioTracks();
	void SubTracks();
	void ChaptersList();
	void VideoMenu();
    void Device_info_Widget();
	
	
	bool B_A_PRESS = false;
	bool B_B_PRESS = false;
	bool B_X_PRESS = false;
	bool B_Y_PRESS = false;
	bool B_DU_PRESS = false;
	bool B_DD_PRESS = false;
	bool B_DL_PRESS = false;
	bool B_DR_PRESS = false;

	bool B_TL_PRESS = false;
	bool B_TR_PRESS = false;


	bool B_R_PRESS = false;
	bool B_L_PRESS = false;
	bool B_ZR_PRESS = false;
	bool B_ZL_PRESS = false;

	bool B_PLUS_PRESS = false;
	bool B_MINUS_PRESS = false;
	
	bool B_AX_L_UP_PRESS = false;
	bool B_AX_L_DOWN_PRESS = false;
	bool B_AX_L_LEFT_PRESS = false;
	bool B_AX_L_RIGHT_PRESS = false;
	
	bool exitLoop = false;
	
	//bool dvdplayback = false;
	float volval = 0;
	
	bool playguitoogle = false;
	
	bool show_videomenu = false;
	bool show_audiotracks = false;
	bool show_subtracks = false;
	bool show_chapters = false;
        
	//uint32_t current_playback_time = 0;
	//uint32_t duration_playback_time = 0;
	
	uint32_t gui_width;
	uint32_t gui_height;
    
    DiscType CurrentDiscType = DiscType::UNKNOWN;
    
    void BackBut();
	
	
};



#endif