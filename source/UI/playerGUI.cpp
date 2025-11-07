#include "appwindows.h"
#include "gui.h"
#include "uiwidgets.h"


bool volume_slider_active = false;

struct AspectRatioData {
    std::string ratio;
    std::string description;
};

const std::vector<AspectRatioData> aspect_ratios = {
        {"16:9", "Widescreen (standard moderno per TV e film)"},
        {"4:3", "Standard (vecchio formato per TV)"},
        {"2.35:1", "Anamorfico (standard cinematografico)"},
        {"1.85:1", "Widescreen (standard cinematografico)",},
        {"2.40:1", "Widescreen (standard cinematografico)"}
    };

void CGUI::PlayerGUI(){
        appWindows::SetupMainWindow();
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.3f, 0.3f, 0.3f, 0.6f));
        ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f*multiplyRes);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 25.0f*multiplyRes);
        
        ImGui::SetNextWindowSize(ImVec2(1240.0f*multiplyRes,(150*multiplyRes))); 
        if (ImGui::Begin("##playerwindow", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse)) {
            ImGui::SetWindowPos(ImVec2(20.0f*multiplyRes,(700*multiplyRes)-(170*multiplyRes)));
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.6f, 0.6f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.6f, 0.8f, 1.0f));
            
            ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX()+20.0f*multiplyRes,ImGui::GetCursorPosY()+25.0f*multiplyRes));
           
            float _posx = ImGui::GetCursorPosX();
            float _posy = ImGui::GetCursorPosY();
            if(Custom_CircleButton("##BACKC",40*multiplyRes,&imgloader->icons.Prev_Chapter_Icon)){
                std::vector<TitleInfo> _titles_list;
                if(libmpv->DVDNAV!=nullptr && libmpv->_playidx >= 0){
                    _titles_list = libmpv->DVDNAV->titles_info;
                }
                if(libmpv->BLURAYNAV!=nullptr && libmpv->_playidx >= 0){
                    _titles_list = libmpv->BLURAYNAV->titles_info;
                }
                if(CurrentDiscType == DiscType::DVD_VIDEO){
                int currchapter = 0;
                if(libmpv->DVDNAV){
                    currchapter = libmpv->DVDNAV->GetChapterIDX(libmpv->_playidx,libmpv->current_playback_time);
                
                }if(libmpv->BLURAYNAV){
                    currchapter = libmpv->BLURAYNAV->GetChapterIDX(libmpv->_playidx,libmpv->current_playback_time);
                
                }
                if(currchapter>0 && currchapter<libmpv->DVDNAV->titles_info[libmpv->_playidx].chapters.size()){
                        currchapter-=1;
                        libmpv->seekToSecond(libmpv->DVDNAV->titles_info[libmpv->_playidx].chapters[currchapter].start);
                    }
                }
                 
            }
            ImGui::SameLine();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX()+5.0f*multiplyRes);
            
            if(Custom_CircleButton("##REW",40*multiplyRes,&imgloader->icons.Rew_Icon)){
                libmpv->seek(-60.0f,false);    
            }
            ImGui::SameLine();
            
            //ImGui::SetCursorPosX(ImGui::GetCursorPosX()+5.0f*multiplyRes);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY()-5.0f*multiplyRes);
            
            if(libmpv->paused){
                if(Custom_CircleButton("##Play",50*multiplyRes,&imgloader->icons.Pause_Icon)){
                    libmpv->Resume();    
                }
            }else{
                if(Custom_CircleButton("##Play",50*multiplyRes,&imgloader->icons.Play_Icon)){
                    libmpv->Pause();    
                }
            }
            ImGui::SameLine();
            ImGui::SameLine();
            //ImGui::SetCursorPosX(ImGui::GetCursorPosX()+5.0f*multiplyRes);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY()-5.0f*multiplyRes);
            if(Custom_CircleButton("##STOP",50*multiplyRes,&imgloader->icons.Stop_Icon)){
                libmpv->Stop();    
            }
            ImGui::SameLine();
            
            if(Custom_CircleButton("##FF",40*multiplyRes,&imgloader->icons.FF_Icon)){
                libmpv->seek(60.0f,false);        
            }
            ImGui::SameLine();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX()+5.0f*multiplyRes);
            
            if(Custom_CircleButton("##NEXTC",40*multiplyRes,&imgloader->icons.Next_Chapter_Icon)){
                int currchapter = 0;
                if(libmpv->DVDNAV){
                    currchapter = libmpv->DVDNAV->GetChapterIDX(libmpv->_playidx,libmpv->current_playback_time);
                
                }if(libmpv->BLURAYNAV){
                    currchapter = libmpv->BLURAYNAV->GetChapterIDX(libmpv->_playidx,libmpv->current_playback_time);
                
                }
                currchapter+=1;
                libmpv->seekToSecond(libmpv->DVDNAV->titles_info[libmpv->_playidx].chapters[currchapter].start);
                        
            }
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 12.0f);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX()+10.0f*multiplyRes);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY()-20.0f*multiplyRes);
            
            /*  Player VIS  */
            _posx = ImGui::GetCursorPosX();
            _posy = ImGui::GetCursorPosY();
            
            ImVec2 _displaysize = ImVec2(500.0f*multiplyRes,80.0f*multiplyRes);
            
            if(CurrentDiscType == DiscType::DVD_VIDEO){
            int _chapidx = libmpv->DVDNAV->GetChapterIDX(libmpv->_playidx,libmpv->current_playback_time);
                std::string chapstr = "Chapter " + std::to_string(_chapidx+1);
                LCD_Display_Widget(chapstr.c_str(),libmpv->current_playback_time*1000,libmpv->duration_playback_time*1000,&_displaysize);
            }else if(CurrentDiscType == DiscType::SVCD){
                LCD_Display_Widget("SVCD",libmpv->current_playback_time*1000,libmpv->duration_playback_time*1000,&_displaysize);
            
            }if(CurrentDiscType == DiscType::VCD){
                LCD_Display_Widget("VCD",libmpv->current_playback_time*1000,libmpv->duration_playback_time*1000,&_displaysize);
            
            }if(CurrentDiscType == DiscType::BLU_RAY){
                int _chapidx = libmpv->BLURAYNAV->GetChapterIDX(libmpv->_playidx,libmpv->current_playback_time);
                std::string chapstr = "Chapter " + std::to_string(_chapidx+1);
                LCD_Display_Widget(chapstr.c_str(),libmpv->current_playback_time*1000,libmpv->duration_playback_time*1000,&_displaysize);
            
            }
            ImGui::PopStyleColor();
            ImGui::PopStyleVar(2);
            
            /*  Player VIS END */
            
            ImGui::SetCursorPosY(_posy+30.0f*multiplyRes);
            ImGui::SetCursorPosX(_posx+520.0f*multiplyRes);
            ImVec2 slider_size = ImVec2(300*multiplyRes,30*multiplyRes);
            ImVec2 slider_minmax = ImVec2(0,100);
            ImVec4 slider_color = ImVec4(0.1f,0.8f,0.1f,1.0f);
            ImVec4 slider_knobcolor = ImVec4(0.4f,0.4f,0.4f,1.0f);
            ImVec4 slider_backcolor = ImVec4(0.1f,0.1f,0.1f,1.0f);
            CustomSlider(&volume_slider_active,"##volslider:",&slider_size,&volval,&slider_minmax,&slider_color,&slider_knobcolor, &slider_backcolor);
            
            
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f*multiplyRes);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX()+20.0f*multiplyRes);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY()+30.0f*multiplyRes);
            ImVec2 downstartpos = ImGui::GetCursorPos();
            if(Custom_ButtonwImage("Video",ImVec2(90*multiplyRes,30*multiplyRes))){
                show_videomenu = true;
            }
            ImGui::SameLine();
            if (Custom_ButtonwImage("Audio",ImVec2(90*multiplyRes,30*multiplyRes))){
                show_audiotracks = true;
            }
            ImGui::SameLine();
            if (Custom_ButtonwImage("Subtitles",ImVec2(100*multiplyRes,30*multiplyRes))){
                show_subtracks = true;
            }
            ImGui::SameLine();
            if (Custom_ButtonwImage("Chapters",ImVec2(100*multiplyRes,30*multiplyRes))){
                show_chapters = true;
                fflush(stdout);
            }
            
            ImGui::SetCursorPos(downstartpos);
            ImGui::SetCursorPosX(1240.0f*multiplyRes-90.0f*multiplyRes);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY()-10.0f*multiplyRes);
            if(CurrentDiscType == DiscType::DVD_VIDEO){
                ImGui::Image((ImTextureID)(intptr_t)imgloader->icons.DVDVideo_Icon.id, ImVec2((60.0f*multiplyRes),imgloader->icons.DVDVideo_Icon.height*(60.0f*multiplyRes)/imgloader->icons.DVDVideo_Icon.width));
            }
            if(CurrentDiscType == DiscType::BLU_RAY){
                ImGui::Image((ImTextureID)(intptr_t)imgloader->icons.BluRay_Icon.id, ImVec2((60.0f*multiplyRes),imgloader->icons.BluRay_Icon.height*(60.0f*multiplyRes)/imgloader->icons.BluRay_Icon.width));
            }
            if(CurrentDiscType == DiscType::SVCD){
                ImGui::Image((ImTextureID)(intptr_t)imgloader->icons.SVCD_Icon.id, ImVec2((60.0f*multiplyRes),imgloader->icons.SVCD_Icon.height*(60.0f*multiplyRes)/imgloader->icons.SVCD_Icon.width));
            }
            if(CurrentDiscType == DiscType::VCD){
                ImGui::Image((ImTextureID)(intptr_t)imgloader->icons.VCD_Icon.id, ImVec2((60.0f*multiplyRes),imgloader->icons.VCD_Icon.height*(60.0f*multiplyRes)/imgloader->icons.VCD_Icon.width));
            }
            
            ImGui::PopStyleColor(2);
            
            ImGui::PopStyleColor(1);
            ImGui::PopStyleVar(1);
            
        }
        
        ImGui::PopStyleVar(5);
        ImGui::PopStyleColor(1);
        appWindows::ExitMainWindow();
    }


void CGUI::AudioTracks(){
    float _myw = 500.0f*multiplyRes;
    float _myh = 300.0f*multiplyRes;
    
    appWindows::SetupOptsWindow(ImVec2(_myw,_myh));
    if (ImGui::Begin("##audiotracks", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse)) {
                std::string titlestr = "Audio Tracks";
                float titlewidth = ImGui::CalcTextSize(titlestr.c_str()).x;
                ImGui::SetCursorPos(ImVec2((_myw-titlewidth)*0.5f,10.0f));
                ImGui::Text(titlestr.c_str());

		if(libmpv!=nullptr){
                        std::vector<TitleInfo> _titles_list;
                        if(libmpv->DVDNAV!=nullptr && libmpv->_playidx >= 0){
                            _titles_list = libmpv->DVDNAV->titles_info;
                        }
                        if(libmpv->BLURAYNAV!=nullptr && libmpv->_playidx >= 0){
                            _titles_list = libmpv->BLURAYNAV->titles_info;
                        }
			if(libmpv !=nullptr && libmpv->_playidx >= 0){
                                static int selected = -1;
                                ImGui::PushStyleColor(ImGuiCol_TableBorderLight, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // Bianco opaco
                                ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(8.0f, 12.0f)); // X=8, Y=12 (default è circa 4,2)

                                ImGuiTableFlags flags = ImGuiTableFlags_Borders |       // Bordi esterni
                                                        ImGuiTableFlags_RowBg;
                                if (ImGui::BeginTable("##audiotable", 1, flags)) { 
                            
                                    for(int i=0;i<_titles_list[libmpv->_playidx].audios.size();i++){
                                            ImGui::TableNextRow(); 
                                            ImGui::TableNextColumn();
                                            std::string _itemid = "##item" + std::to_string(i);
                                            ImVec2 start_pos = ImGui::GetCursorPos();
                                            if(ImGui::Selectable(_itemid.c_str(), libmpv->audioID ==  _titles_list[libmpv->_playidx].audios[i].id,ImGuiSelectableFlags_SpanAllColumns)){
                                                    libmpv->set_audio_track(_titles_list[libmpv->_playidx].audios[i].id	);
                                                    selected = i;
                                            }
                                            ImGui::SetCursorPos(start_pos);
                                            ImGui::Text("id:%d %s - %s",_titles_list[libmpv->_playidx].audios[i].id,_titles_list[libmpv->_playidx].audios[i].language.c_str(),_titles_list[libmpv->_playidx].audios[i].codec.c_str());


                                    }
                                    ImGui::TableNextRow(); 
                                    ImGui::TableNextColumn();
                                    ImVec2 start_pos = ImGui::GetCursorPos();
                                    
                                    if(ImGui::Selectable("##disableaudio", false)){
                                            libmpv->set_audio_track(0);
                                            selected = -1;
                                    }
                                    ImGui::SetCursorPos(start_pos);
                                    ImGui::Text("Disable Audio");
                                    ImGui::EndTable();
                                }
                                
                                ImGui::PopStyleColor();
                                ImGui::PopStyleVar();
				
			}
		}
		ImGui::SetCursorPos(ImVec2(10.0f,_myh-40.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 26.0f); // X=8, Y=12 (default è circa 4,2)

                if(ImGui::Button("Close",ImVec2(_myw-20.0f,30.0f))){
                    show_audiotracks = false;
                }      
                ImGui::PopStyleVar();
	}

	appWindows::ExitOptsWindow();
}

void CGUI::SubTracks(){
    float _myw = 500.0f*multiplyRes;
    float _myh = 300.0f*multiplyRes;
    
    appWindows::SetupOptsWindow(ImVec2(_myw,_myh));
    if (ImGui::Begin("##subtracks", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse)) {
                std::string titlestr = "Subtitle Tracks";
                float titlewidth = ImGui::CalcTextSize(titlestr.c_str()).x;
                ImGui::SetCursorPos(ImVec2((_myw-titlewidth)*0.5f,10.0f));
                ImGui::Text(titlestr.c_str());
        
		if(libmpv!=nullptr){
			std::vector<TitleInfo> _titles_list;
                        if(libmpv->DVDNAV!=nullptr && libmpv->_playidx >= 0){
                            _titles_list = libmpv->DVDNAV->titles_info;
                        }
                        if(libmpv->BLURAYNAV!=nullptr && libmpv->_playidx >= 0){
                            _titles_list = libmpv->BLURAYNAV->titles_info;
                        }
			if(libmpv !=nullptr && libmpv->_playidx >= 0){
                                static int selected = -1;
                                ImGui::PushStyleColor(ImGuiCol_TableBorderLight, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // Bianco opaco
                                ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(8.0f, 12.0f)); // X=8, Y=12 (default è circa 4,2)
                                ImGuiTableFlags flags = ImGuiTableFlags_Borders |       // Bordi esterni
                                                        ImGuiTableFlags_RowBg;
                                if (ImGui::BeginTable("##audiotable", 1, flags)) { 
                                    for(int i=0;i<_titles_list[libmpv->_playidx].subtitles.size();i++){
                                            ImGui::TableNextRow(); 
                                            ImGui::TableNextColumn();
                                            
                                            std::string _itemid = "##item" + std::to_string(i);
                                            ImVec2 start_pos = ImGui::GetCursorPos();
                                            if(ImGui::Selectable(_itemid.c_str(), libmpv->subID == _titles_list[libmpv->_playidx].subtitles[i].id)){
                                                    libmpv->set_sub_track(_titles_list[libmpv->_playidx].subtitles[i].id);
                                            }
                                            ImGui::SetCursorPos(start_pos);
                                            ImGui::Text("id:%d %s",_titles_list[libmpv->_playidx].subtitles[i].id,_titles_list[libmpv->_playidx].subtitles[i].language.c_str());


                                    }
                                    ImGui::TableNextRow(); 
                                    ImGui::TableNextColumn();
                                    ImVec2 start_pos = ImGui::GetCursorPos();
                                    
                                    if(ImGui::Selectable("##disablesub", false)){
                                            libmpv->set_sub_track(0);
                                    }
                                    ImGui::SetCursorPos(start_pos);
                                    ImGui::Text("Disable Sub");
                                    ImGui::EndTable();
                                }
                                ImGui::PopStyleColor();
                                ImGui::PopStyleVar();
					
			}
		}
		ImGui::SetCursorPos(ImVec2(10.0f,_myh-40.0f));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 26.0f); // X=8, Y=12 (default è circa 4,2)

                if(ImGui::Button("Close",ImVec2(_myw-20.0f,30.0f))){
                    show_subtracks = false;
                }      
                ImGui::PopStyleVar();

        }

	appWindows::ExitOptsWindow();
}


void CGUI::ChaptersList(){
    float _myw = 500.0f*multiplyRes;
    float _myh = 300.0f*multiplyRes;
    
    appWindows::SetupOptsWindow(ImVec2(_myw,_myh));
    
    if (ImGui::Begin("##chaplist", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse)) {
        std::string titlestr = "Chapters List";
        float titlewidth = ImGui::CalcTextSize(titlestr.c_str()).x;
        ImGui::SetCursorPos(ImVec2((_myw-titlewidth)*0.5f,10.0f));
        ImGui::Text(titlestr.c_str());
        if(libmpv!=nullptr){
                std::vector<TitleInfo> _titles_list;
                if(libmpv->DVDNAV!=nullptr && libmpv->_playidx >= 0){
                    _titles_list = libmpv->DVDNAV->titles_info;
                }
                if(libmpv->BLURAYNAV!=nullptr && libmpv->_playidx >= 0){
                     _titles_list = libmpv->BLURAYNAV->titles_info;
                }
                if(libmpv !=nullptr && libmpv->_playidx >= 0){
                    static int selected = -1;
                    /*
                    ImVec2 outer_size = ImVec2(0,200);
                    if (ImGui::BeginTable("##chaptertable", 1, flags, outer_size)) { 
                        for(int i=0;i<_titles_list[libmpv->_playidx].chapters.size();i++){
                            ImGui::TableNextRow(); 
                            ImGui::TableNextColumn();
                            std::string _itemid = "##chapitem" + std::to_string(i);
                            //std::string entryname = _titles_list[libmpv->_playidx].audio_tracks[i].lang + std::string(" - ") + _titles_list[libmpv->_playidx].audio_tracks[i].format;
                            std::string entryname = "Chapter " + std::to_string(_titles_list[libmpv->_playidx].chapters[i].num)+ " " + millisecondsToTimeStringFast(_titles_list[libmpv->_playidx].chapters[i].start*1000);
                            
                            ImVec2 start_pos = ImGui::GetCursorPos();
                            if(ImGui::Selectable(_itemid.c_str(), false)){
                                libmpv->seekToSecond(_titles_list[libmpv->_playidx].chapters[i].start);
                            }
                            ImGui::SetCursorPos(start_pos);
                            ImGui::Text("%s",entryname.c_str());
                            
                        }
                        ImGui::EndTable();
                    }
                    */
                    for(int i=0;i<_titles_list[libmpv->_playidx].chapters.size();i++){
                        std::string _itemid = "##chapitem" + std::to_string(i);
                        ImVec2 _startpos = ImGui::GetCursorPos();
                        int _chapidx = -1;
                        if(libmpv->BLURAYNAV){
                            _chapidx = libmpv->BLURAYNAV->GetChapterIDX(libmpv->_playidx,libmpv->current_playback_time);
                        }
                        if(libmpv->DVDNAV){
                            _chapidx = libmpv->DVDNAV->GetChapterIDX(libmpv->_playidx,libmpv->current_playback_time);
                        }
                        if(ImGui::Selectable(_itemid.c_str(), i==_chapidx)){
                                libmpv->seekToSecond(_titles_list[libmpv->_playidx].chapters[i].start);
                        }
                        ImGui::SameLine();
                        ImGui::SetCursorPos(_startpos);
                        std::string entryname = "Chapter " + std::to_string(_titles_list[libmpv->_playidx].chapters[i].num)+ " " + millisecondsToTimeStringFast(_titles_list[libmpv->_playidx].chapters[i].start*1000);
                            
                        ImGui::Text(entryname.c_str());
                    }
                    
                
					
            }
        }
        
    }

    appWindows::ExitOptsWindow();
    
}


void CGUI::VideoMenu(){
    float _myw = 500.0f*multiplyRes;
    float _myh = 300.0f*multiplyRes;
    
    appWindows::SetupOptsWindow(ImVec2(_myw,_myh));
    if (ImGui::Begin("##videomenu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse)) {
        std::vector<TitleInfo> _titles_list;
        if(libmpv->DVDNAV!=nullptr && libmpv->_playidx >= 0){
            _titles_list = libmpv->DVDNAV->titles_info;
        }
        if(libmpv->BLURAYNAV!=nullptr && libmpv->_playidx >= 0){
            _titles_list = libmpv->BLURAYNAV->titles_info;
        }
        if(libmpv->SVCDNAV!=nullptr && libmpv->_playidx >= 0){
            _titles_list = libmpv->SVCDNAV->titles_info;
        }
        if(_titles_list[libmpv->_playidx].videos.size()>0){
            std::string videorestext = std::to_string(_titles_list[libmpv->_playidx].videos[0].width) + "x" + std::to_string(_titles_list[libmpv->_playidx].videos[0].height) + " " + _titles_list[libmpv->_playidx].videos[0].codec + " " + _titles_list[libmpv->_playidx].videos[0].format_name;
            float titlewidth = ImGui::CalcTextSize(videorestext.c_str()).x;
            ImGui::SetCursorPos(ImVec2((_myw-titlewidth)*0.5f,ImGui::GetCursorPosY()));
            ImGui::Text(videorestext.c_str());    
        }
         
        std::string titlestr = "Video Aspect";
        float titlewidth = ImGui::CalcTextSize(titlestr.c_str()).x;
        ImGui::SetCursorPos(ImVec2((_myw-titlewidth)*0.5f,ImGui::GetCursorPosY()));
        ImGui::Text(titlestr.c_str());
        
        if(libmpv!=nullptr){
            if(libmpv->_playidx >= 0){
                static int selected = -1;
                ImGui::PushStyleColor(ImGuiCol_TableBorderLight, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // Bianco opaco
                ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(8.0f, 12.0f)); // X=8, Y=12 (default è circa 4,2)
                ImGuiTableFlags flags = ImGuiTableFlags_Borders |       // Bordi esterni
                ImGuiTableFlags_RowBg;
                if (ImGui::BeginTable("##chaptertable", 1, flags)) {
                    for(int i=0;i<aspect_ratios.size();i++){
                        ImGui::TableNextRow(); 
                        ImGui::TableNextColumn();
                        std::string _itemid = "##aratio" + std::to_string(i);
                        std::string entryname = aspect_ratios[i].ratio;
                        ImVec2 start_pos = ImGui::GetCursorPos();
                        if(ImGui::Selectable(_itemid.c_str(), false)){
                            libmpv->setAspectRatio(aspect_ratios[i].ratio.c_str());    
                        }
                        ImGui::SetCursorPos(start_pos);
                        ImGui::Text("%s",entryname.c_str());    
                    }
                    
                    ImGui::EndTable();
                }
                ImGui::PopStyleColor();
                ImGui::PopStyleVar();
            }
        }
        //ImGui::SetCursorPos(ImVec2(10.0f,_myh-40.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 26.0f); // X=8, Y=12 (default è circa 4,2)
                    
        if(ImGui::Button("Close",ImVec2(_myw-20.0f,30.0f))){
            show_videomenu = false;
        }      
        ImGui::PopStyleVar();
    }
    
    appWindows::ExitOptsWindow();
}