#ifndef NXMP_IMGLOADER_H
#define NXMP_IMGLOADER_H


#include "nxmp-render.h"
#include "deko3d.hpp"
#include "stb_image.h"



typedef struct {
	/* Home Icons */
	Texture Play_Icon;
	Texture Pause_Icon;
	Texture Stop_Icon;
	
	Texture Rew_Icon;
	Texture FF_Icon;
	Texture Next_Chapter_Icon;
	Texture Prev_Chapter_Icon;
    
    Texture Eject_Icon;
    Texture Mount_Icon;
    Texture Optical_Disc_Icon;
    
    Texture VCD_Icon;
    Texture SVCD_Icon;
    Texture BluRay_Icon;
    Texture DVDVideo_Icon;
		
} nxlaser_icon_struct;
	


class CImgLoader{
public:
		CImgLoader();
		void LoadBaseTextures(std::string basepath);
		~CImgLoader();
		nxlaser_icon_struct icons;
		
		NXMPRenderer *Renderer;
		
		Texture OpenImageFile(std::string path);
		Texture OpenImageMemory(unsigned char *_img_data,int _size);
		Texture OpenRAWImageMemory(unsigned char *_img_data,int _w,int _h);

};




#endif