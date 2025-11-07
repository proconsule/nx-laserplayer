#include "uiwidgets.h"

#include <string>
#include <stdint.h>



ColorStates GenerateColorStatesHSV(ImVec4 base_color, float hover_brightness, float active_brightness)
{
    ColorStates states;
    float h, s, v;
    
    // Converti in HSV
    ImGui::ColorConvertRGBtoHSV(base_color.x, base_color.y, base_color.z, h, s, v);
    
    // Colore normale
    states.normal = base_color;
    
    // Colore hovered (aumenta brightness)
    float hovered_v = v + hover_brightness;
    if (hovered_v > 1.0f) hovered_v = 1.0f;
    ImGui::ColorConvertHSVtoRGB(h, s, hovered_v, states.hovered.x, states.hovered.y, states.hovered.z);
    states.hovered.w = base_color.w;
    
    // Colore active (diminuisce brightness)
    float active_v = v + active_brightness;
    if (active_v < 0.0f) active_v = 0.0f;
    ImGui::ColorConvertHSVtoRGB(h, s, active_v, states.active.x, states.active.y, states.active.z);
    states.active.w = base_color.w;
    
    return states;
}

void CustomSlider(bool * _inuse,const char* label, ImVec2 *size, float* value, ImVec2 *minmax, ImVec4 *mycolor, ImVec4 *myknobcolor, ImVec4 *mybackcolor)
{
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 slider_size(size->x, size->y);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImGuiStyle& style = ImGui::GetStyle();
    
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return;
    ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID(label);
    
    const ImVec2 _pos = window->DC.CursorPos;
    const ImRect bb(_pos, ImVec2(pos.x + slider_size.x, pos.y + slider_size.y + g.Style.FramePadding.y * 2.0f));

    
    ImGui::ItemAdd(bb, id, &bb);
    if(ImGui::InvisibleButton(label, slider_size)){
        *_inuse = !*_inuse;
    }
    if(*_inuse)ImGui::SetFocusID(id,window);
    
    
    bool hovered = ImGui::IsItemHovered();
    bool active = ImGui::IsItemActive();
    bool focused = ImGui::IsItemFocused();
    
    if ((focused || active) && (hovered || active) && *_inuse) {
        float range = minmax->y - minmax->x;
        float step = range * 0.01f; // Passo dell'1%
        float fast_step = range * 0.1f; // Passo del 10% con Shift
        
        bool changed = false;
        
        // Input da tastiera
        if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow)) {
            *value -= fast_step;
            changed = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) {
            *value += fast_step;
            changed = true;
        }
        
        // Input da gamepad (D-pad)
        if (ImGui::IsKeyPressed(ImGuiKey_GamepadDpadLeft)) {
            *value -= fast_step;
            changed = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_GamepadDpadRight)) {
            *value += fast_step;
            changed = true;
        }
        
        // Stick analogici del gamepad (solo se attivo)
        if (active) {
            // Stick sinistro orizzontale
            if (ImGui::IsKeyDown(ImGuiKey_GamepadLStickLeft)) {
                *value -= step * ImGui::GetIO().DeltaTime * 5.0f;
                changed = true;
            }
            if (ImGui::IsKeyDown(ImGuiKey_GamepadLStickRight)) {
                *value += step * ImGui::GetIO().DeltaTime * 5.0f;
                changed = true;
            }
        }
        
        if (changed) {
            // Clamp
            if (*value < minmax->x) *value = minmax->x;
            if (*value > minmax->y) *value = minmax->y;
        }
    }
    
    ColorStates mybackstates = GenerateColorStatesHSV(*mybackcolor);
    ImU32 bg_color = ImGui::ColorConvertFloat4ToU32(
        active ? mybackstates.active : 
        (hovered || focused) ? mybackstates.hovered : 
        mybackstates.normal
    );
    
    ColorStates mycolorstates = GenerateColorStatesHSV(*mycolor);
    
    ImU32 fill_color = ImGui::ColorConvertFloat4ToU32(
        active ? mycolorstates.active : 
        (hovered || focused) ? mycolorstates.hovered : 
        mycolorstates.normal
    );
    
    if(*_inuse){
        *myknobcolor = ImVec4(1.0f,0.0f,0.0f,1.0f);
    }
    ColorStates myknobstates = GenerateColorStatesHSV(*myknobcolor);
    
    
    ImU32 knob_color = ImGui::ColorConvertFloat4ToU32(
        active ? myknobstates.active : 
        (hovered || focused) ? myknobstates.hovered : 
        myknobstates.normal
    );
    
    knob_color = (knob_color & 0x00FFFFFF) | 0xFF000000;
    
    // Bordo per indicare il focus
    if (focused && !active) {
        ImU32 focus_color = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_NavHighlight]);
        draw_list->AddRect(pos, ImVec2(pos.x + slider_size.x, pos.y + slider_size.y), 
                          focus_color, 20.0f, 0, 2.0f);
    }
    
    draw_list->AddRectFilled(pos, ImVec2(pos.x + slider_size.x, pos.y + slider_size.y), 
                            bg_color, 20.0f);
    
    float fill_width = (*value - minmax->x) / (minmax->y - minmax->x) * slider_size.x;
    if (fill_width > 0) {
        draw_list->AddRectFilled(pos, ImVec2(pos.x + fill_width, pos.y + slider_size.y),
                                fill_color, 20.0f);
    }
    
    char value_text[32];
    snprintf(value_text, sizeof(value_text), "%.0f%%", *value);
    
    ImVec2 text_size = ImGui::CalcTextSize(value_text);
    ImVec2 text_pos = ImVec2(
        pos.x + (slider_size.x - text_size.x) * 0.5f,  
        pos.y + (slider_size.y - text_size.y) * 0.5f  
    );
    
    ImU32 text_color = IM_COL32(255, 255, 255, 255); 
    
    if (text_pos.x + text_size.x * 0.5f < pos.x + fill_width) {
        float brightness = mycolor->x * 0.299f + mycolor->y * 0.587f + mycolor->z * 0.114f;
        text_color = brightness > 0.5f ? IM_COL32(0, 0, 0, 255) : IM_COL32(255, 255, 255, 255);
    } else {
        text_color = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_Text]);
    }
    
    draw_list->AddText(text_pos, text_color, value_text);
    
    float knob_pos = pos.x + fill_width - 5;
    draw_list->AddCircleFilled(ImVec2(knob_pos + 5, pos.y + slider_size.y * 0.5f), 
                              slider_size.y * 0.5f, knob_color);
    
}


void LCD_Display_Widget(const char *_mainlabel,uint32_t _curr_msec,uint32_t _duration_msec,ImVec2 *_size,Texture  *_disc_img){
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.6f));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 12.0f*multiplyRes);
    
    ImGui::BeginChild("##playervis_id", ImVec2(_size->x, _size->y), false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
    ImVec2 _startpos = ImGui::GetCursorPos();
    ImVec2 _startscreenpos = ImGui::GetCursorScreenPos();       
    
    float labewidth = ImGui::CalcTextSize(_mainlabel).x;
    
    ImGui::SetCursorPosX(ImGui::GetCursorPosX()+10.0f*multiplyRes + (_size->x-labewidth)*0.5f);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY()+10.0f*multiplyRes);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
    ImGui::Text(_mainlabel);
    
    std::string currpos = millisecondsToTimeStringFast(_curr_msec);
    std::string maxpos = millisecondsToTimeStringFast(_duration_msec);
    std::string timestring = currpos + " - " + maxpos;
            
    float timestringwidth = ImGui::CalcTextSize(timestring.c_str()).x;
    
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImVec2 myimgpos = window->DC.CursorPos;         
    ImGui::SetCursorPosY(_startpos.y+45.0f*multiplyRes);
    ImGui::SetCursorPosX(_startpos.x+(_size->x)-timestringwidth-10.0f*multiplyRes);
    ImGui::Text(timestring.c_str());
    ImGui::PopStyleColor(2);
    ImGui::PopStyleVar();
    
    ImGui::SetCursorPosY(_startpos.y);
    ImGui::SetCursorPosX(_startpos.x);
    
    
    myimgpos = ImVec2(myimgpos.x+_size->x-70.0f*multiplyRes,myimgpos.y-10.0f*multiplyRes);
    if (_disc_img) {
        ImVec2 image_size = ImVec2(60.0f*multiplyRes,_disc_img->height*(60.0f*multiplyRes)/_disc_img->width);
        draw_list->AddImage((void *)(intptr_t)_disc_img->id, myimgpos, ImVec2(myimgpos.x + image_size.x, myimgpos.y + image_size.y));
    }
     
    
    
    float _perc = _curr_msec*100.0f/_duration_msec;
    static ImColor greencol(0.f, 1.f, 0.f, 1.f);
    static ImColor barbackcol(0.3f, 0.3f, 0.3f, 1.f);
    //ImDrawList* draw_list = ImGui::GetWindowDrawList();
    float barwidth = 290.0f*multiplyRes*_perc/100.0f;
    draw_list->AddRectFilled(ImVec2(_startscreenpos.x+10.0f*multiplyRes, _startscreenpos.y+45.0f*multiplyRes), ImVec2(_startscreenpos.x+300*multiplyRes, _startscreenpos.y+65.0f*multiplyRes), barbackcol, 3.f, 0 );
    draw_list->AddRectFilled(ImVec2(_startscreenpos.x+10.0f*multiplyRes, _startscreenpos.y+45.0f*multiplyRes), ImVec2(_startscreenpos.x+10*multiplyRes+barwidth, _startscreenpos.y+65.0f*multiplyRes), greencol, 3.f, 0 );
    
    
    ImGui::EndChild();
    
    
    
}

bool Custom_Button(const char* _label, ImVec2 _size,float _scale) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *ImGui::GetCurrentContext();
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(_label);
    
    ImVec2 label_size = ImGui::CalcTextSize(_label);
    label_size.x = label_size.x*_scale;
    label_size.y = label_size.y*_scale;
    ImVec2 pos = window->DC.CursorPos;
    
    if (_size.x <= 0.0f) _size.x = label_size.x + style.FramePadding.x * 2.0f;
    if (_size.y <= 0.0f) _size.y = label_size.y + style.FramePadding.y * 2.0f;
    
    const ImRect bb(pos, ImVec2(pos.x + _size.x, pos.y + _size.y));
    ImGui::ItemSize(_size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id)) return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

    // Colori per l'effetto rilievo
    ImU32 bg_color;
    ImU32 border_light, border_dark;
    
    if (held) {
        bg_color = ImGui::GetColorU32(ImGuiCol_ButtonActive);
        border_light = ImGui::GetColorU32(ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
        border_dark = ImGui::GetColorU32(ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
    } else {
        bg_color = hovered ? ImGui::GetColorU32(ImGuiCol_ButtonHovered) : ImGui::GetColorU32(ImGuiCol_Button);
        border_light = ImGui::GetColorU32(ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
        border_dark = ImGui::GetColorU32(ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    }

    ImDrawList* draw_list = window->DrawList;
    
    // Sfondo
    draw_list->AddRectFilled(bb.Min, bb.Max, bg_color, style.FrameRounding);
    
    ImVec2 text_pos;
    text_pos.x = bb.Min.x + (_size.x - label_size.x) * 0.5f;
    text_pos.y = bb.Min.y + (_size.y - label_size.y) * 0.5f;
    
    if (held) {
        text_pos.x += 1.0f;
        text_pos.y += 1.0f;
    }
    
    ImFont* font = ImGui::GetFont();
    float imfontsize = ImGui::CalcTextSize("A").y;
    draw_list->AddText(font, imfontsize*_scale, text_pos, ImGui::GetColorU32(ImGuiCol_Text), _label, nullptr);
    
    return pressed;
}

bool Custom_ButtonwImage(const char* _label, ImVec2 _size, float _scale,Texture * _buttexture,ImVec4 *mybackcolor) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *ImGui::GetCurrentContext();
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(_label);

    ImFont* font = ImGui::GetFont();
    float imfontsize = ImGui::CalcTextSize("A").y;
    ImVec2 label_size = ImGui::CalcTextSize(_label);
    label_size.x = label_size.x * _scale;
    label_size.y = label_size.y * _scale;
    
    // Dimensione dell'immagine
    
    // Calcola la larghezza totale del contenuto (testo + immagine)
    float content_width = label_size.x;
    if (_buttexture) {
        ImVec2 image_size = ImVec2(label_size.y, _buttexture->height*label_size.y/_buttexture->width); // Usa l'altezza del testo come dimensione dell'immagine (quadrata)
    
        content_width += image_size.x + style.ItemSpacing.x; // Aggiungi la larghezza dell'immagine e uno spazio
    }

    ImVec2 pos = window->DC.CursorPos;

    if (_size.x <= 0.0f) _size.x = content_width + style.FramePadding.x * 2.0f;
    if (_size.y <= 0.0f) _size.y = label_size.y + style.FramePadding.y * 2.0f;

    const ImRect bb(pos, ImVec2(pos.x + _size.x, pos.y + _size.y));
    ImGui::ItemSize(_size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id)) return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

    ImU32 bg_color;
    ColorStates mybackstates = GenerateColorStatesHSV(style.Colors[ImGuiCol_Button]);
    if(mybackcolor){
        mybackstates = GenerateColorStatesHSV(*mybackcolor);
    }
    if (held) {
        bg_color = ImGui::GetColorU32(ImGui::ColorConvertFloat4ToU32(mybackstates.active));
    } else {
        bg_color = hovered ? ImGui::ColorConvertFloat4ToU32(mybackstates.hovered) : ImGui::ColorConvertFloat4ToU32(mybackstates.normal);
    }

    ImDrawList* draw_list = window->DrawList;
    draw_list->AddRectFilled(bb.Min, bb.Max, bg_color, style.FrameRounding);
    
    if(hovered){
        ImVec2 _h_min = bb.Min;
        ImVec2 _h_max = bb.Max;
        _h_min.x = _h_min.x-8.0f;
        _h_min.y = _h_min.y-8.0f;
        _h_max.x = _h_max.x+8.0f;
        _h_max.y = _h_max.y+8.0f;
        
        draw_list->AddRect(_h_min, _h_max, IM_COL32(255, 255, 255, 255), style.FrameRounding,0,2.0f);
    }
    else if(held){
        ImVec2 _h_min = bb.Min;
        ImVec2 _h_max = bb.Max;
        _h_min.x = _h_min.x-12.0f;
        _h_min.y = _h_min.y-12.0f;
        _h_max.x = _h_max.x+12.0f;
        _h_max.y = _h_max.y+12.0f;
        
        draw_list->AddRect(_h_min, _h_max, IM_COL32(255, 255, 255, 255), style.FrameRounding,0,2.0f);
    }
    
    
    ImVec2 total_size = ImVec2(content_width, label_size.y);
    ImVec2 text_pos;
    ImVec2 image_pos;

    ImVec2 content_start_pos = ImVec2(
        bb.Min.x + (_size.x - total_size.x) * 0.5f,
        bb.Min.y + (_size.y - total_size.y) * 0.5f
    );
    
    image_pos = content_start_pos;
    text_pos = ImVec2(content_start_pos.x, content_start_pos.y);

    if (_buttexture) {
        ImVec2 image_size = ImVec2(label_size.y, _buttexture->height*label_size.y/_buttexture->width); // Usa l'altezza del testo come dimensione dell'immagine (quadrata)
    
        image_pos = content_start_pos;
        text_pos.x = content_start_pos.x + image_size.x + style.ItemSpacing.x; // Sposta il testo dopo l'immagine
    }

    if (held) {
        image_pos.x += 1.0f;
        image_pos.y += 1.0f;
        text_pos.x += 1.0f;
        text_pos.y += 1.0f;
    }

    if (_buttexture) {
        ImVec2 image_size = ImVec2(label_size.y, _buttexture->height*label_size.y/_buttexture->width); // Usa l'altezza del testo come dimensione dell'immagine (quadrata)
        image_pos = ImVec2(image_pos.x,image_pos.y+(label_size.y-image_size.y)*0.5f/*-style.ItemSpacing.y*2*/);
        draw_list->AddImage((void *)(intptr_t)_buttexture->id, image_pos, ImVec2(image_pos.x + image_size.x, image_pos.y + image_size.y));
    }

    draw_list->AddText(font, imfontsize * _scale, text_pos, ImGui::GetColorU32(ImGuiCol_Text), _label, nullptr);

    return pressed;
}

bool Custom_ImageButton(const char* _hiddenlabel, ImVec2 _size,Texture * _buttexture,ImVec4 *mybackcolor) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *ImGui::GetCurrentContext();
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(_hiddenlabel);

    ImFont* font = ImGui::GetFont();
    float imfontsize = ImGui::CalcTextSize("A").y;
    ImVec2 label_size = _size;
    label_size.x = 0.0f;
    label_size.y = label_size.y-style.ItemSpacing.y*2.0;
    
    
    // Dimensione dell'immagine
    
    // Calcola la larghezza totale del contenuto (testo + immagine)
    float content_width = label_size.x;
    if (_buttexture) {
        ImVec2 image_size = ImVec2(label_size.y, _buttexture->height*label_size.y/_buttexture->width); // Usa l'altezza del testo come dimensione dell'immagine (quadrata)
    
        content_width += image_size.x; // Aggiungi la larghezza dell'immagine e uno spazio
    }

    ImVec2 pos = window->DC.CursorPos;

    if (_size.x <= 0.0f) _size.x = content_width + style.FramePadding.x * 2.0f;
    if (_size.y <= 0.0f) _size.y = label_size.y + style.FramePadding.y * 2.0f;

    const ImRect bb(pos, ImVec2(pos.x + _size.x, pos.y + _size.y));
    ImGui::ItemSize(_size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id)) return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

    ImU32 bg_color;
    ColorStates mybackstates = GenerateColorStatesHSV(style.Colors[ImGuiCol_Button]);
    if(mybackcolor){
        mybackstates = GenerateColorStatesHSV(*mybackcolor);
    }
    if (held) {
        bg_color = ImGui::GetColorU32(ImGui::ColorConvertFloat4ToU32(mybackstates.active));
    } else {
        bg_color = hovered ? ImGui::ColorConvertFloat4ToU32(mybackstates.hovered) : ImGui::ColorConvertFloat4ToU32(mybackstates.normal);
    }

    ImDrawList* draw_list = window->DrawList;
    draw_list->AddRectFilled(bb.Min, bb.Max, bg_color, style.FrameRounding);
    
    if(hovered){
        ImVec2 _h_min = bb.Min;
        ImVec2 _h_max = bb.Max;
        _h_min.x = _h_min.x-8.0f;
        _h_min.y = _h_min.y-8.0f;
        _h_max.x = _h_max.x+8.0f;
        _h_max.y = _h_max.y+8.0f;
        
        draw_list->AddRect(_h_min, _h_max, IM_COL32(255, 255, 255, 255), style.FrameRounding,0,2.0f);
    }
    else if(held){
        ImVec2 _h_min = bb.Min;
        ImVec2 _h_max = bb.Max;
        _h_min.x = _h_min.x-12.0f;
        _h_min.y = _h_min.y-12.0f;
        _h_max.x = _h_max.x+12.0f;
        _h_max.y = _h_max.y+12.0f;
        
        draw_list->AddRect(_h_min, _h_max, IM_COL32(255, 255, 255, 255), style.FrameRounding,0,2.0f);
    }
    
    
    ImVec2 total_size = ImVec2(content_width, label_size.y);
    ImVec2 text_pos;
    ImVec2 image_pos;

    ImVec2 content_start_pos = ImVec2(
        bb.Min.x + (_size.x - total_size.x) * 0.5f,
        bb.Min.y + (_size.y - total_size.y) * 0.5f
    );
    
    image_pos = content_start_pos;
    text_pos = ImVec2(content_start_pos.x, content_start_pos.y);

    if (_buttexture) {
        ImVec2 image_size = ImVec2(label_size.y, _buttexture->height*label_size.y/_buttexture->width); // Usa l'altezza del testo come dimensione dell'immagine (quadrata)
    
        image_pos = content_start_pos;
        text_pos.x = content_start_pos.x + image_size.x + style.ItemSpacing.x; // Sposta il testo dopo l'immagine
    }

    if (held) {
        image_pos.x += 1.0f;
        image_pos.y += 1.0f;
        text_pos.x += 1.0f;
        text_pos.y += 1.0f;
    }

    if (_buttexture) {
        ImVec2 image_size = ImVec2(label_size.y, _buttexture->height*label_size.y/_buttexture->width); // Usa l'altezza del testo come dimensione dell'immagine (quadrata)
        image_pos = ImVec2(image_pos.x,image_pos.y+(label_size.y-image_size.y)*0.5f/*-style.ItemSpacing.y*2*/);
        draw_list->AddImage((void *)(intptr_t)_buttexture->id, image_pos, ImVec2(image_pos.x + image_size.x, image_pos.y + image_size.y));
    }

    //draw_list->AddText(font, imfontsize * _scale, text_pos, ImGui::GetColorU32(ImGuiCol_Text), _label, nullptr);

    return pressed;
}


bool Custom_CircleButton(const char* _label,float _diameter,Texture * _buttexture){
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *ImGui::GetCurrentContext();
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(_label);
    float content_width = _diameter;
    
    ImVec2 pos = window->DC.CursorPos;
    
    const ImRect bb(pos, ImVec2(pos.x + _diameter, pos.y + _diameter));
    ImVec2 _size = ImVec2(_diameter,_diameter);
    ImGui::ItemSize(_size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id)) return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

    ImU32 bg_color;
    if (held) {
        bg_color = ImGui::GetColorU32(ImGuiCol_ButtonActive);
    } else {
        bg_color = hovered ? ImGui::GetColorU32(ImGuiCol_ButtonHovered) : ImGui::GetColorU32(ImGuiCol_Button);
    }

    ImDrawList* draw_list = window->DrawList;
    draw_list->AddRectFilled(bb.Min, bb.Max, bg_color, style.FrameRounding);
    
    if(hovered){
        ImVec2 _h_min = bb.Min;
        ImVec2 _h_max = bb.Max;
        _h_min.x = _h_min.x-8.0f;
        _h_min.y = _h_min.y-8.0f;
        _h_max.x = _h_max.x+8.0f;
        _h_max.y = _h_max.y+8.0f;
        
        draw_list->AddRect(_h_min, _h_max, IM_COL32(255, 255, 255, 255), style.FrameRounding,0,2.0f);
    }
    else if(held){
        ImVec2 _h_min = bb.Min;
        ImVec2 _h_max = bb.Max;
        _h_min.x = _h_min.x-12.0f;
        _h_min.y = _h_min.y-12.0f;
        _h_max.x = _h_max.x+12.0f;
        _h_max.y = _h_max.y+12.0f;
        
        draw_list->AddRect(_h_min, _h_max, IM_COL32(255, 255, 255, 255), style.FrameRounding,0,2.0f);
    }
    
    
    ImVec2 total_size = ImVec2(content_width, _diameter);
    ImVec2 text_pos;
    ImVec2 image_pos;

    ImVec2 content_start_pos = ImVec2(
        bb.Min.x + (_size.x - total_size.x) * 0.5f,
        bb.Min.y + (_size.y - total_size.y) * 0.5f
    );
    
    image_pos = content_start_pos;
    text_pos = ImVec2(content_start_pos.x, content_start_pos.y);

    if (held) {
        image_pos.x += 1.0f;
        image_pos.y += 1.0f;
        text_pos.x += 1.0f;
        text_pos.y += 1.0f;
    }

    //ImFont* font = ImGui::GetFont();
    //float imfontsize = ImGui::CalcTextSize("A").y;
    //draw_list->AddText(font, imfontsize , text_pos, ImGui::GetColorU32(ImGuiCol_Text), _label, nullptr);
    draw_list->AddImage((void *)(intptr_t)_buttexture->id, image_pos, ImVec2(image_pos.x + _diameter, image_pos.y + _diameter));
    
    
    return pressed;
    
}
