#include <algorithm>
#include <iostream>

#include "UI/appwindows.h"
#include "gui.h"


#define is_bit_set(val, bit_no) (((val) >> (bit_no)) & 1)

inline uint64_t bit_set(uint64_t number, unsigned int n) {
		return number | ((unsigned int)1 << n);
	}

enum BUT_FLAGS{
		BUT_A,
		BUT_B,
		BUT_X,
		BUT_Y,
		
		BUT_DUP,
		BUT_DDOWN,
		BUT_DLEFT,
		BUT_DRIGHT,
		
		BUT_L,
		BUT_R,
		BUT_ZL,
		BUT_ZR,
		
		BUT_TR,
		BUT_TL,
		
		BUT_PLUS,
		BUT_MINUS,
		
		B_AX_L_UP,
		B_AX_L_DOWN,
		B_AX_L_LEFT,
		B_AX_L_RIGHT,
		
	
		B_AX_R_UP,
		B_AX_R_DOWN,
		B_AX_R_LEFT,
		B_AX_R_RIGHT,
		
	};

CGUI::CGUI(uint32_t _w,uint32_t _h){
	gui_width = _w;
    gui_height = _h;
	
}


void CGUI::RenderLoop(){
	CheckDiscType();
	
	while(exitLoop == false){
		HandleEvents();
		HandleLayers();
		HandleRender();
	}
}

void CGUI::HandleEvents(){
	
		libmpv->processEvents();
        
		uint64_t event_ret = Process_UI_Events(std::chrono::system_clock::now());
		
		if(event_ret>0){
			
			if(is_bit_set(event_ret,B_AX_R_UP)){
				
				ImGui::GetIO().AddKeyEvent(ImGuiKey_GamepadLStickUp,true);
				
			}
			
			if(is_bit_set(event_ret,B_AX_R_DOWN)){
				
				ImGui::GetIO().AddKeyEvent(ImGuiKey_GamepadLStickDown,true);
				
			}
			
			if(is_bit_set(event_ret,B_AX_L_UP)){
				
			}
			
			if(is_bit_set(event_ret,B_AX_L_DOWN)){
				
			}
			
			if(is_bit_set(event_ret,B_AX_L_LEFT)){
				
				ImGui::GetIO().AddKeyEvent(ImGuiKey_GamepadDpadLeft,true);
			}
			
			if(is_bit_set(event_ret,B_AX_L_RIGHT)){
				
				ImGui::GetIO().AddKeyEvent(ImGuiKey_GamepadDpadRight,true);
			}
			
			if(is_bit_set(event_ret,BUT_Y)){
				show_audiotracks = false;
				show_subtracks = false;
			}
			
			if (is_bit_set(event_ret,BUT_DUP) || is_bit_set(event_ret,B_AX_L_UP)){
				
			}
			
			if (is_bit_set(event_ret,BUT_DDOWN) || is_bit_set(event_ret,B_AX_L_DOWN)){
				
			}
			
			if (is_bit_set(event_ret,BUT_X)){
				
			}
			
			if (is_bit_set(event_ret,BUT_A)){
				if(libmpv->disc_playback){
                    playgui_show = true;
                }
			}
			if (is_bit_set(event_ret,BUT_B)){
				BackBut();
			}
			
			if (is_bit_set(event_ret,BUT_DRIGHT)){
				
			}
			
			if (is_bit_set(event_ret,BUT_DLEFT)){
				
			}
			
			if (is_bit_set(event_ret,BUT_ZR)){
						
			}
			if (is_bit_set(event_ret,BUT_ZL)){
				
			}
			
			if (is_bit_set(event_ret,BUT_R)){
				libmpv->seek(60.0f,true);		
			}
			
			if (is_bit_set(event_ret,BUT_L)){
				libmpv->seek(-60.0f,true);	
			}
			
			if (is_bit_set(event_ret,BUT_TL) && is_bit_set(event_ret,BUT_TR)){
				
			}
			
			if (is_bit_set(event_ret,BUT_TL)){
				
						
			}
			if (is_bit_set(event_ret,BUT_TR)){
				
						
			}
				
			if (is_bit_set(event_ret,BUT_PLUS)){
                if(libmpv->disc_playback==false){
                    exitLoop = true;
                }
			}
            
            if (is_bit_set(event_ret,BUT_MINUS)){
                /*
                if(!usbdvd->usbdvd_ctx.drive.drive_found){
                    if(sel_subclass == 0){
                        sel_subclass = 1;
                    }else{
                        sel_subclass = 0;
                    }
                    delete usbdvd;
                    usbdvd = new CUSBDVD(sel_subclass,true);
                }
                */
			}

			if(is_bit_set(event_ret,B_AX_R_DOWN)){
				
			}
			if(is_bit_set(event_ret,B_AX_R_UP)){
				
			}
		}else{
			//ImGui::GetIO().AddKeyEvent(ImGuiKey_GamepadLStickUp,false);
			//ImGui::GetIO().AddKeyEvent(ImGuiKey_GamepadLStickDown,false);
		}
	
	}
void CGUI::HandleLayers(){
	ImGui::NewFrame();
	if(libmpv->disc_playback==false){
        MainMenuGUI();
        if(show_dvdmenu){
            DVDMenu();
        }
        if(show_bdmenu){
            BDMenu();
        }
        if(show_options){
            optionsUI();
        }
    }
    if(libmpv->disc_playback && playgui_show){
        PlayerGUI();
        if(show_videomenu){
            VideoMenu();
        }
        if(show_audiotracks){
            AudioTracks();
        }
        if(show_subtracks){
            SubTracks();
        }
        if(show_chapters){
            ChaptersList();
        }
    }
			
}
void CGUI::HandleRender(){
		Renderer->GetFBImage();
		Renderer->begin_frame();
		Renderer->UpdateVO();
		ImGui::Render();
		Renderer->end_frame();
		ImGui::EndFrame();
	
}


uint64_t CGUI::Process_UI_Events(std::chrono::time_point<std::chrono::system_clock> myeventtime){
		
		uint64_t ret_event = 0;
		
		uint64_t keydown =  imgui::nx::newFrame();
		
		if (keydown & HidNpadButton_A && !B_A_PRESS){
			ret_event = bit_set(ret_event,BUT_A);
		}
		if (keydown & HidNpadButton_B && !B_B_PRESS){
			ret_event = bit_set(ret_event,BUT_B);
		}
		if (keydown & HidNpadButton_Y && !B_Y_PRESS){
			ret_event = bit_set(ret_event,BUT_Y);
		}
		if (keydown & HidNpadButton_X && !B_X_PRESS){
			ret_event = bit_set(ret_event,BUT_X);
		}
		if (keydown & HidNpadButton_Up && !B_DU_PRESS){
			ret_event = bit_set(ret_event,BUT_DUP);
		}
		if (keydown & HidNpadButton_Down && !B_DD_PRESS){
			ret_event = bit_set(ret_event,BUT_DDOWN);
		}
		if (keydown & HidNpadButton_Left && !B_DL_PRESS){
			ret_event = bit_set(ret_event,BUT_DLEFT);
		}
		if (keydown & HidNpadButton_Right && !B_DR_PRESS){
			ret_event = bit_set(ret_event,BUT_DRIGHT);
		}
		if (keydown & HidNpadButton_L && !B_L_PRESS){
			ret_event = bit_set(ret_event,BUT_L);
		}
		if (keydown & HidNpadButton_R && !B_R_PRESS){
			ret_event = bit_set(ret_event,BUT_R);
		}
		if (keydown & HidNpadButton_ZL && !B_ZL_PRESS){
			ret_event = bit_set(ret_event,BUT_ZL);
		}
		if (keydown & HidNpadButton_ZR && !B_ZR_PRESS){
			ret_event = bit_set(ret_event,BUT_ZR);
		}
		if (keydown & HidNpadButton_StickL && !B_TL_PRESS){
			ret_event = bit_set(ret_event,BUT_TL);
		}
		if (keydown & HidNpadButton_StickR && !B_TR_PRESS){
			ret_event = bit_set(ret_event,BUT_TR);
		}
		if (keydown & HidNpadButton_StickRUp){
			ret_event = bit_set(ret_event,B_AX_R_UP);
		}
		if (keydown & HidNpadButton_StickRDown){
			ret_event = bit_set(ret_event,B_AX_R_DOWN);
		}
		if (keydown & HidNpadButton_Minus && !B_MINUS_PRESS){
			ret_event = bit_set(ret_event,BUT_MINUS);
		}
		if (keydown & HidNpadButton_Plus && !B_PLUS_PRESS){
			ret_event = bit_set(ret_event,BUT_PLUS);
		}
		   
		   
		B_ZL_PRESS = keydown & HidNpadButton_ZL;
		B_ZR_PRESS = keydown & HidNpadButton_ZR;
			
		B_A_PRESS = keydown & HidNpadButton_A;
		B_X_PRESS = keydown & HidNpadButton_X;
		B_B_PRESS = keydown & HidNpadButton_B;
		B_Y_PRESS = keydown & HidNpadButton_Y;
		B_DD_PRESS = keydown & HidNpadButton_Down;
		B_DU_PRESS = keydown & HidNpadButton_Up;
			
		B_DL_PRESS = keydown & HidNpadButton_Left;
		B_DR_PRESS = keydown & HidNpadButton_Right;

		B_R_PRESS = keydown & HidNpadButton_R;
		B_L_PRESS = keydown & HidNpadButton_L;
		B_TR_PRESS = keydown & HidNpadButton_StickR;
		B_TL_PRESS = keydown & HidNpadButton_StickL;

		B_PLUS_PRESS =  keydown & HidNpadButton_Plus;
		B_MINUS_PRESS = keydown & HidNpadButton_Minus;
			
			
			
		B_AX_L_UP_PRESS = keydown & HidNpadButton_StickLUp;
		B_AX_L_DOWN_PRESS = keydown & HidNpadButton_StickLDown;
		B_AX_L_LEFT_PRESS = keydown & HidNpadButton_StickLLeft;
		B_AX_L_RIGHT_PRESS = keydown & HidNpadButton_StickLRight;
	
	return ret_event;
	}
    
    
void CGUI::BackBut(){
    if(show_audiotracks){
        show_audiotracks=false;
    }else if(show_subtracks){
        show_subtracks=false;
    }else if(this->show_chapters){
        show_chapters=false;
    }else if(this->show_videomenu){
        show_videomenu=false;
    }else if(playgui_show){
        playgui_show=false;
    }else if(show_dvdmenu){
        show_dvdmenu=false;
    }else if(show_bdmenu){
        show_bdmenu=false;
    }
}

void CGUI::CheckDiscType(){
    if(usbdvd->usbdvd_ctx.fs.mounted){
    CurrentDiscType = getDiscType(usbdvd->usbdvd_ctx.fs.mountpoint + std::string("/") );
    }
}