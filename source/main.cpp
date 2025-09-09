#include <stdio.h>
#include <ctype.h>
#include <utility>

#include "nxmp-render.h"
#include "imgui_nx.h"

#include "logger.h"

#include <switch.h>

#include "gui.h"
#include "usbdvd.h"
#include "imgloader.h"

#define NDEBUG 0


extern "C" u32 __nx_applet_exit_mode, __nx_nv_service_type, __nx_nv_transfermem_size;
bool                g_application_mode;

std::string nxmpTitle = std::string("NXMP v") + std::to_string(VERSION_MAJOR) + std::string(".") + std::to_string(VERSION_MINOR) + std::string(".") + std::to_string(VERSION_MICRO);


/* Classes */


libMpv *libmpv = nullptr;
NXMPRenderer *Renderer = nullptr;
CUSBDVD *usbdvd = nullptr;

CGUI* GUI = nullptr;
CImgLoader *imgloader;


float multiplyRes = 1.0f;
float initScale = 1.0f;
int initSize = 55;
int batteryPercent = 0;
bool isHandheld = true;
bool clockoc = false;
int newResW = 1280;
int newResH = 720;

std::string tempKbUrl = "https://";

unsigned int app_exit_mode = 0;

float currFontsize = 20.0f; 

/* VARS */


ImVec4 Text_color = ImVec4(255, 255, 255, 1.00f);
ImVec4 Active_color = ImVec4(0, 255, 203, 1.00f);
ImVec4 Disabled_color = ImVec4(0.41f, 0.40f, 0.40f, 1.00f);
ImVec4 Window_Bg_color = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
ImVec4 OptsTab_Bg_color = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
ImVec4 Popup_Bg_color = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
ImVec4 NavHover_color = ImVec4(0.0f, 0.0f, 0.0f, 0.20f);
ImVec4 HeaderHover_color = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
ImVec4 Button_color = ImVec4(0.0f, 0.0f, 0.0f, 0.00f);
ImVec4 ButtonActive_color = ImVec4(0, 255, 203, 0.30f);

float CurrentVolume = 0.0f;


void SetDarkTheme(){
		Text_color = ImVec4(255, 255, 255, 1.00f);
		Active_color = ImVec4(0, 255, 203, 1.00f);
		Disabled_color = ImVec4(0.41f, 0.40f, 0.40f, 1.00f);
		Window_Bg_color = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
		OptsTab_Bg_color = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
		Popup_Bg_color = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
		NavHover_color = ImVec4(0.0f, 0.0f, 0.0f, 0.20f);
		HeaderHover_color = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
		Button_color = ImVec4(0.0f, 0.0f, 0.0f, 0.00f);
		ButtonActive_color = ImVec4(0, 255, 203, 0.30f);
		
		ImGuiStyle* style = &ImGui::GetStyle();
		style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.62f);
		style->Colors[ImGuiCol_Text] = Text_color;
		
	}
	void SetLightTheme(){
		Text_color = ImVec4(0, 0, 0, 1.00f);
		Active_color = ImVec4(0, 0, 255, 1.00f);
		Disabled_color = ImVec4(0.41f, 0.40f, 0.40f, 1.00f);
		Window_Bg_color = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
		OptsTab_Bg_color = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
		Popup_Bg_color = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
		NavHover_color = ImVec4(1.0f, 1.0f, 1.0f, 0.20f);
		HeaderHover_color = ImVec4(0.98f, 0.98f, 0.98f, 1.00f);
		Button_color = ImVec4(0.0f, 0.0f, 0.0f, 0.00f);
		ButtonActive_color = ImVec4(0, 0, 255, 0.30f);
		
		ImGuiStyle* style = &ImGui::GetStyle();
		style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.62f);
		style->Colors[ImGuiCol_Text] = Text_color;
		
	}

void SetColorTheme(int themecolor){
		
		ColorSetId currentTheme;
		Result rc = setsysInitialize();
		if (R_SUCCEEDED(rc)) {
		setsysGetColorSetId(&currentTheme);
		}
		
		if(themecolor == 0){
			SetDarkTheme();
		}else if(themecolor == 1){
			SetLightTheme();
		} else{
			if(currentTheme == 1){
				SetDarkTheme();
			}else{
				SetLightTheme();
			}
		}
		setsysExit();
	}


void DeallocateExtern(){
	
	
	
}



int main(int argc, const  char **argv) {
	

/**************** This init taken from SwitchWave https://github.com/averne/SwitchWave  make it applet mode friendly  ************************/	

	svcSetThreadPriority(CUR_THREAD_HANDLE, 0x20);
	
	appletLockExit();
	auto at = appletGetAppletType();
    g_application_mode = at == AppletType_Application || at == AppletType_SystemApplication;
	__nx_nv_service_type     = NvServiceType_Factory;
    __nx_nv_transfermem_size = (g_application_mode ? 16 : 3) * 0x100000;
	
	
/*************************************************************************************/
	
	romfsInit();
	auto socket_conf = *socketGetDefaultInitConfig();
    socket_conf.bsd_service_type = BsdServiceType_Auto;
    socketInitialize(&socket_conf);
	
	appletSetMediaPlaybackState(true);
	
	


#ifdef NDEBUG
	nxlinkStdio();
#endif	
	NXLOG::DEBUGLOG("Starting NXMP\n");
	


	AppletOperationMode stus=appletGetOperationMode();
	if (stus == AppletOperationMode_Handheld) {
		NXLOG::DEBUGLOG("Handheld Mode\n");
		isHandheld=true;
		newResW = 1280;
		newResH = 720;
		multiplyRes = 1.0f;
		currFontsize = 18.0f;
	}
	if (stus == AppletOperationMode_Console) {
		NXLOG::DEBUGLOG("Docked Mode\n");
		isHandheld=false;
		newResW = 1920;
		newResH = 1080;
		multiplyRes = 1.5f;
		currFontsize = 27.0f;
	}
	
	NXLOG::loglevel = 2;
	
	
	appletInitializeGamePlayRecording();
    appletSetWirelessPriorityMode(AppletWirelessPriorityMode_OptimizedForWlan);
	
	
	
	
	Renderer = new NXMPRenderer();
	Renderer->Init_ImGui();
	if(!Renderer->initialize()){
		
	}
	Renderer->Init_ImGui_deko3d();
	NXLOG::DEBUGLOG("Renderer Init ok\n");
	

	imgui::nx::setEnableTouch(true);
	
	
	NXLOG::DEBUGLOG("Imgui Init OK\n");
	
	
	
	libmpv = new libMpv("mpv");
	Renderer->create_mpv_render_context(libmpv);
	NXLOG::DEBUGLOG("MPV Context Done\n");
	
	imgloader = new CImgLoader();
	imgloader->Renderer = Renderer;
	imgloader->LoadBaseTextures("romfs:");
	
	
	
	usbdvd = new CUSBDVD();
	printf("USBDVD INIT DONE\r\n");
	
	
	GUI = new CGUI(newResW,newResH);
	GUI->Renderer = Renderer;
	GUI->libmpv = libmpv;
	GUI->usbdvd = usbdvd;
	GUI->imgloader = imgloader;
	
    
	
	GUI->RenderLoop();
	app_exit_mode = 0;
	__nx_applet_exit_mode = app_exit_mode;
	
	
    
    Renderer->destroy_mpv_render_context();
    NXLOG::DEBUGLOG("Destroy Renderer\n");
	
	delete libmpv;
	libmpv = nullptr;
	
	
	NXLOG::DEBUGLOG("Ending MPV\n");	
    delete imgloader;
	delete Renderer;	

	delete usbdvd;
    nvExit();
	appletSetMediaPlaybackState(false);
	romfsExit();
	NXLOG::DEBUGLOG("nx-laserplayer End\n");
    socketExit();
	appletUnlockExit();
	
    return 0;
}

