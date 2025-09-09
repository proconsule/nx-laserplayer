#ifndef UIWIDGETS_H
#define UIWIDGETS_H

#include "imgui.h"
#include "imgui_internal.h"
#include <cstdint>
#include <string>

struct ColorStates {
    ImVec4 normal;
    ImVec4 hovered;
    ImVec4 active;
};

ColorStates GenerateColorStatesHSV(ImVec4 base_color, float hover_brightness = 0.1f, float active_brightness = -0.1f);
void CustomSlider(bool *_inuse,const char* label, ImVec2 *size, float* value, ImVec2 *minmax, ImVec4 *mycolor, ImVec4 *myknobcolor, ImVec4 *mybackcolor);
void LCD_Display_Widget(const char *_mainlabel,uint32_t _curr_msec,uint32_t _duration_msec,ImVec2 *_size);
std::string millisecondsToTimeStringFast(long long milliseconds);
bool Custom_Button(const char* label, ImVec2 size = ImVec2(0, 0),float _scale = 1.0f);
bool Custom_ButtonwImage(const char* _label, ImVec2 size = ImVec2(0, 0),float _scale = 1.0f, void * texid = NULL  );

#endif /* UIWIDGETS_H */

