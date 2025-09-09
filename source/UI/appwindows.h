#ifndef NXMP_WINDOWS_H
#define NXMP_WINDOWS_H

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"
#include "nxmp-render.h"
#include "usbdvd.h"


extern int newResW;
extern int newResH;
extern float multiplyRes;
extern bool isHandheld;

extern ImVec4 Text_color;
extern ImVec4 Active_color;
extern ImVec4 Disabled_color;
	
extern ImVec4 NavHover_color;
extern ImVec4 HeaderHover_color;
extern ImVec4 Window_Bg_color;
extern ImVec4 OptsTab_Bg_color;
extern ImVec4 Popup_Bg_color;
extern ImVec4 Button_color;
extern ImVec4 ButtonActive_color;

using namespace ImGui;


namespace appWindows {

	inline void SetupMainWindow(void) {
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(1280.0f*multiplyRes, 720.0f*multiplyRes), ImGuiCond_Always);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg,Window_Bg_color);
    };
	
	inline void SetupOptsWindow(ImVec2 _size) {
        ImGuiIO& io{ ::ImGui::GetIO() };
        float xpos = ((io.DisplaySize.x-_size.x)*0.5f);
        float ypos = ((io.DisplaySize.y-_size.y)*0.5f);
        
        ImGui::SetNextWindowPos(ImVec2(xpos,ypos), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(_size.x, _size.y), ImGuiCond_Always);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 20.0f);
        ImGui::PushStyleColor(ImGuiCol_WindowBg,Window_Bg_color);
    };
	
	
    inline void SetupWindow(void) {
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(1280.0f*multiplyRes, 720.0f*multiplyRes), ImGuiCond_Always);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg,Window_Bg_color);
    };

    inline void ExitMainWindow(void) {
        ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
        
    };
	
	inline void ExitOptsWindow(void) {
        ImGui::End();
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
    };
	
	inline void ExitWindow(void) {
        ImGui::End();
        ImGui::PopStyleVar();
		ImGui::PopStyleColor();
    };
	
	
	
	
}


#endif