#include "gui.h"
#include "uiwidgets.h"
#include "appwindows.h"

#include "utils.h"

void CGUI::optionsUI(){
    float _myw = 500.0f*multiplyRes;
    float _myh = 300.0f*multiplyRes;
    
    appWindows::SetupOptsWindow(ImVec2(_myw,_myh));
    if (ImGui::Begin("##optionsui", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse)) {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
        nx_laserplayer_config_struct  * tmpconfig = iniparser->getConfig();
        std::string titlestr = "Options";
        float titlewidth = ImGui::CalcTextSize(titlestr.c_str()).x;
        ImGui::SetCursorPos(ImVec2((_myw-titlewidth)*0.5f,10.0f));
        ImGui::Text(titlestr.c_str());
        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("##optionstab", tab_bar_flags)){
            if (ImGui::BeginTabItem("Audio")){
                int aidx = getlnagIDX(tmpconfig->audio_lang);
                if(aidx ==-1)aidx = 0;
                std::string preview_value = dvd_audio_languages[aidx] +  " - " + dvd_audio_languages_extended[aidx];
                if (ImGui::BeginCombo("Audio Language", preview_value.c_str(), 0))
                {
                    for (int n = 0; n < dvd_audio_languages.size(); n++)
                    {
                        const bool is_selected = (aidx == n);
                        std::string entrymame = dvd_audio_languages[n] +  " - " + dvd_audio_languages_extended[n];
                        if (ImGui::Selectable(entrymame.c_str(), is_selected)){
                            tmpconfig->audio_lang = dvd_audio_languages[n];
                        }

                        // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Subtitle")){
                int sidx = getlnagIDX(tmpconfig->sub_lang);
                if(sidx ==-1)sidx = 0;
                
                std::string preview_value = dvd_audio_languages[sidx] +  " - " + dvd_audio_languages_extended[sidx];
                if (ImGui::BeginCombo("Subtitle Language", preview_value.c_str(), 0))
                {
                    for (int n = 0; n < dvd_audio_languages.size(); n++)
                    {
                        const bool is_selected = (sidx == n);
                        std::string entrymame = dvd_audio_languages[n] +  " - " + dvd_audio_languages_extended[n];
                        
                        if (ImGui::Selectable(entrymame.c_str(), is_selected))
                            tmpconfig->sub_lang = dvd_audio_languages[n];

                        // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
                ImGui::EndTabItem();
            }
            
            ImGui::EndTabBar();
            
            
            ImGuiContext& g = *ImGui::GetCurrentContext();
            const ImGuiStyle& style = g.Style;
            ImVec2 buttons_size = ImVec2(250.0f*multiplyRes-style.ItemSpacing.x*2,30.0f*multiplyRes);
            ImGui::SetCursorPos(ImVec2(style.ItemSpacing.x,_myh-style.ItemSpacing.y-40.0f*multiplyRes));
            
            ImVec4 mytestcolor = ImVec4(0.7f,0.0f,0.0f,1.0f);
            if(Custom_ButtonwImage("Save",buttons_size,1.0f,&imgloader->icons.Save_Icon,&mytestcolor)){
                iniparser->saveSettings(tmpconfig);
                show_options = false;
            }
            ImVec4 mytestcolor2 = ImVec4(0.3f,0.3f,0.3f,1.0f);
            ImGui::SameLine();
            if(Custom_ButtonwImage("Cancel",buttons_size,1.0f,NULL,&mytestcolor2)){
                iniparser->RevertChanges();
                show_options = false;
            }
        }
        ImGui::PopStyleVar();
    }
    
    
    appWindows::ExitOptsWindow();
    
}