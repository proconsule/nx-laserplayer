#include "appwindows.h"
#include "uiwidgets.h"
#include "gui.h"



void CGUI::MainMenuGUI() {
        appWindows::SetupMainWindow();

		
		if (ImGui::Begin("nx-laserplayer", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse)) {
			ImGui::SetWindowFontScale(2.0f);
                        std::string title = "nx-laserplayer";
			ImGuiIO &io = ImGui::GetIO();
                        
                        
                        
                        
			float windowwidth = ImGui::GetWindowWidth();
                        float windowheight = ImGui::GetWindowHeight();
			float titlewidth = ImGui::CalcTextSize(title.c_str()).x;
			ImGui::SetCursorPosX((windowwidth-titlewidth)*0.5f);
                        
			ImGui::Text(title.c_str());
                        ImGui::SetWindowFontScale(1.0f);
                        
			ImGui::SetCursorPosY(ImGui::GetCursorPosY()+10.0f*multiplyRes);
                        
			if(usbdvd->usbdvd_ctx.drive.drive_found){
				Device_info_Widget();
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
                                
                ImGui::SetCursorPosX(30.0f*multiplyRes);
                ImGui::SetCursorPosY(windowheight-70.0f*multiplyRes);
                                
                                
                if(!usbdvd->usbdvd_ctx.fs.mounted){
                    float _butstartposX = ImGui::GetCursorPosX();
                    float _butstartposY = ImGui::GetCursorPosY();
                                    
                                    
                    if (Custom_ButtonwImage("Mount",ImVec2(400*multiplyRes,50*multiplyRes),2.0f,(void *)(intptr_t)imgloader->icons.Mount_Icon.id)){
                        usbdvd->MountDisc();
                        if(usbdvd->usbdvd_ctx.fs.mounted){
                            CurrentDiscType = getDiscType(usbdvd->usbdvd_ctx.fs.mountpoint + std::string("/"));
                                            
                        }
                    }
                    ImGui::SameLine();
                    ImVec2 endpos = ImGui::GetCursorPos();
                                    
                    ImGui::SetCursorPosX(endpos.x);
                    ImGui::SetCursorPosY(endpos.y);
                }
                if(usbdvd->usbdvd_ctx.fs.mounted){
                    float _butstartposX = ImGui::GetCursorPosX();
                    float _butstartposY = ImGui::GetCursorPosY();
                    if (Custom_ButtonwImage("Play Disc",ImVec2(400*multiplyRes,50*multiplyRes),2.0f,(void *)(intptr_t)imgloader->icons.Optical_Disc_Icon.id)){
                        std::string openpath = usbdvd->usbdvd_ctx.fs.mountpoint;
                        if(CurrentDiscType == DiscType::BLU_RAY){
                            libmpv->loadBluRay(openpath+ "/");
                        }else if(CurrentDiscType == DiscType::DVD_VIDEO){
                            usbdvd->Cache_IFO_Files();
                            libmpv->loadDVD(openpath+ "/VIDEO_TS/");
                        }else if(CurrentDiscType == DiscType::SVCD){
                            libmpv->loadSVCD(openpath + "/");
                        }else if(CurrentDiscType == DiscType::VCD){
                            libmpv->loadVCD(openpath + "/");
                        }
                      
                    }
                                    
                    ImGui::SameLine();
                    ImVec2 endpos = ImGui::GetCursorPos();
                                    
									
                    _butstartposX = ImGui::GetCursorPosX();
                    _butstartposY = ImGui::GetCursorPosY();
                    ImGui::BeginDisabled();
                    if (Custom_ButtonwImage("DVD Menu",ImVec2(400*multiplyRes,50*multiplyRes),2.0f)){
                                       
                    }
                    ImGui::SameLine();
                    endpos = ImGui::GetCursorPos();
                                    
                        ImGui::EndDisabled();
                                    
                }else{
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX()+400.0f*multiplyRes);
                }
                                
                float _butstartposX = ImGui::GetCursorPosX();
                float _butstartposY = ImGui::GetCursorPosY();
                    
                if (Custom_ButtonwImage("Eject",ImVec2(400*multiplyRes,50*multiplyRes),2.0f,(void *)(intptr_t)imgloader->icons.Eject_Icon.id)){
                    usbdvd->Eject();
                }
                ImGui::PopStyleColor(1);
                ImGui::PopStyleVar(2);
			
            }else{
                ImGui::SetWindowFontScale(2.0f);
                std::string drivetotstr = "No USB Drive Found";
                float drivestrwidth = ImGui::CalcTextSize(drivetotstr.c_str()).x;
                ImGui::SetCursorPosX((windowwidth-drivestrwidth)*0.5f);
                                
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
                ImGui::Text("No USB Drive Found");
                ImGui::PopStyleColor();
                ImGui::SetWindowFontScale(1.0f);
            }
			
		}
		appWindows::ExitMainWindow();
		
	}

    
void CGUI::Device_info_Widget(){
    float windowwidth = ImGui::GetWindowWidth();
    float windowheight = ImGui::GetWindowHeight();
    ImVec2 outer_size = ImVec2(500.0f*multiplyRes, 240.0f*multiplyRes);
    ImGui::SetCursorPosX((windowwidth-500.0f*multiplyRes)*0.5f);
        
    if (ImGui::BeginTable("table_scrolly", 2, ImGuiTableFlags_ScrollY|ImGuiTableFlags_NoHostExtendX, outer_size))
    {
        std::string drivestr = std::string(usbdvd->usbdvd_ctx.drive.vendor_id) + " " + std::string(usbdvd->usbdvd_ctx.drive.product_id);
        //std::string drivetotstr = "USB Drive: " + drivestr;
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("USB Device");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f),drivestr.c_str());
         
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Disc Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f),usbdvd->usbdvd_ctx.drive.disc_type);
        if(usbdvd->usbdvd_ctx.fs.mounted){
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("FS Type");
            ImGui::TableSetColumnIndex(1);
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f),usbdvd->usbdvd_ctx.fs.disc_fstype);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Mountpoint");
            ImGui::TableSetColumnIndex(1);
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f),usbdvd->usbdvd_ctx.fs.mountpoint);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Video Disc Type");
            ImGui::TableSetColumnIndex(1);
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f),"%s",DiscTypeToString(CurrentDiscType).c_str());
                    
            if(CurrentDiscType == DiscType::DVD_VIDEO){
                if(usbdvd->usbdvd_ctx.dvd_protection.CSS){
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("CSS");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f),"Disc is CSS protected");
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Key Decryption");
                    ImGui::TableSetColumnIndex(1);
                    if(usbdvd->usbdvd_ctx.dvd_protection.decss_msecs == 0){
                       ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f),"%0.2f seconds",usbdvd->usbdvd_ctx.dvd_protection.decss_msecs/1000.0f);
                       ImGui::SameLine();
                       ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f),"(Cached)");
                       
                     
                    }else{
                       ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f),"%0.2f seconds",usbdvd->usbdvd_ctx.dvd_protection.decss_msecs/1000.0f);
                    }
                    
                    
                }else{
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("CSS");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f),"None");
                                       
                }
            }
        }
     
       
        ImGui::EndTable();
    }
   
    
}