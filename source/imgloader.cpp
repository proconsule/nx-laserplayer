#include "imgloader.h"
#include "logger.h"


#include "stb_image.h"
#include <turbojpeg.h>

enum{
	JPGFILE,
	PNGFILE,
	BMPFILE,
	WEBPFILE,
	GIFFILE
	
};



CImgLoader::CImgLoader(){


	

}


void CImgLoader::LoadBaseTextures(std::string basepath){
	
	NXLOG::DEBUGLOG("LOADING TEXTURES\n");

	icons.Play_Icon = Renderer->load_texture(basepath+"/player/play.png",DkImageFormat_RGBA8_Unorm, 0);
	icons.Pause_Icon = Renderer->load_texture(basepath+"/player/pause.png",DkImageFormat_RGBA8_Unorm, 0);
	icons.Stop_Icon = Renderer->load_texture(basepath+"/player/stop.png",DkImageFormat_RGBA8_Unorm, 0);

	icons.Rew_Icon = Renderer->load_texture(basepath+"/player/rew.png",DkImageFormat_RGBA8_Unorm, 0);
	icons.FF_Icon = Renderer->load_texture(basepath+"/player/ff.png",DkImageFormat_RGBA8_Unorm, 0);
	icons.Next_Chapter_Icon = Renderer->load_texture(basepath+"/player/next_chapter.png",DkImageFormat_RGBA8_Unorm, 0);
	icons.Prev_Chapter_Icon = Renderer->load_texture(basepath+"/player/prev_chapter.png",DkImageFormat_RGBA8_Unorm, 0);

    icons.Eject_Icon = Renderer->load_texture(basepath+"/dvdeject.png",DkImageFormat_RGBA8_Unorm, 0);
	icons.Optical_Disc_Icon = Renderer->load_texture(basepath+"/optical_disc.png",DkImageFormat_RGBA8_Unorm, 0);
	icons.Mount_Icon = Renderer->load_texture(basepath+"/dvdmount.png",DkImageFormat_RGBA8_Unorm, 0);

    icons.DVDVideo_Icon = Renderer->load_texture(basepath+"/dvd_video_icon.png",DkImageFormat_RGBA8_Unorm, 0);
	icons.BluRay_Icon = Renderer->load_texture(basepath+"/blu_ray_icon.png",DkImageFormat_RGBA8_Unorm, 0);
	icons.SVCD_Icon = Renderer->load_texture(basepath+"/svcd_icon.png",DkImageFormat_RGBA8_Unorm, 0);
    icons.VCD_Icon = Renderer->load_texture(basepath+"/vcd_icon.png",DkImageFormat_RGBA8_Unorm, 0);

    
    icons.Save_Icon = Renderer->load_texture(basepath+"/save.png",DkImageFormat_RGBA8_Unorm, 0);
    icons.Options_Icon = Renderer->load_texture(basepath+"/options.png",DkImageFormat_RGBA8_Unorm, 0);

	NXLOG::DEBUGLOG("DONE\r\n");

	
}


CImgLoader::~CImgLoader(){


	
	Renderer->unregister_texture(icons.Play_Icon);
	Renderer->unregister_texture(icons.Pause_Icon);
	Renderer->unregister_texture(icons.Stop_Icon);
	

	
	Renderer->unregister_texture(icons.Rew_Icon);
	Renderer->unregister_texture(icons.FF_Icon);
	Renderer->unregister_texture(icons.Next_Chapter_Icon);
	Renderer->unregister_texture(icons.Prev_Chapter_Icon);
	
    Renderer->unregister_texture(icons.Eject_Icon);
	Renderer->unregister_texture(icons.Optical_Disc_Icon);
	Renderer->unregister_texture(icons.Mount_Icon);
	
    
    Renderer->unregister_texture(icons.DVDVideo_Icon);
	Renderer->unregister_texture(icons.BluRay_Icon);
	Renderer->unregister_texture(icons.SVCD_Icon);
	Renderer->unregister_texture(icons.VCD_Icon);
	
    Renderer->unregister_texture(icons.Save_Icon);
	Renderer->unregister_texture(icons.Options_Icon);
	
    
	
	NXLOG::DEBUGLOG("Unregister Textures\r\n");
	
}

int guessImageMemoryFormat(unsigned char *_img_data){
	int guessret = -1;
	if(_img_data[0]=='G' && _img_data[1]=='I' && _img_data[2]=='F' && _img_data[3]=='8') {
		guessret = GIFFILE;
	}
	else if(_img_data[0]==0xff && _img_data[1]==0xd8){
		guessret = JPGFILE;
	}
	else if(_img_data[0]==0x89 && _img_data[1]==0x50 && _img_data[2]==0x4e && _img_data[3]==0x47) {
		guessret = PNGFILE;
	}
	else if(_img_data[0]==0x42 && _img_data[1]==0x4d) {
		guessret = BMPFILE;
	}
	
	
	return guessret;
}	


int guessImageFormat(std::string path){
	char testread[5];
	FILE *fptr = fopen(path.c_str(), "rb");
	int guessret = -1;
	
	
	
	if(fgets(testread,5,fptr)) {
		testread[4]=0;
		if(!strcmp((char *)testread,"GIF8")) {
			guessret = GIFFILE;
		}
		else if(testread[0]==0xff && testread[1]==0xd8){
			guessret = JPGFILE;
		}
		else if(testread[0]==0x89 && testread[1]==0x50 && testread[2]==0x4e && testread[3]==0x47) {
			guessret = PNGFILE;
		}
		else if(testread[0]==0x42 && testread[1]==0x4d) {
			guessret = BMPFILE;
		}
	}
	fclose(fptr);
	return guessret;
}	


unsigned char * jpg_decode(unsigned char *_img_data,int _size,int *width, int *height, int *channels){
	
	tjhandle _jpegDecompressor = tjInitDecompress();

	int jpegSubsamp;
	tjDecompressHeader2(_jpegDecompressor, _img_data, _size, width, height, &jpegSubsamp);

	int img_w = *width;
	int img_h = *height;

	unsigned char * buffer = (unsigned char *)malloc(img_w*img_h*4);
	*channels = 4;
	tjDecompress2(_jpegDecompressor, _img_data, _size, buffer, img_w, 0/*pitch*/, img_h, TJPF_RGBA, TJFLAG_FASTDCT);

	tjDestroy(_jpegDecompressor);
	return buffer;
}


Texture CImgLoader::OpenImageMemory(unsigned char *_img_data,int _size){
	
	int myformat = guessImageMemoryFormat(_img_data);
	if(myformat == -1){
		return {};
	}
	
	if(myformat == PNGFILE || myformat == BMPFILE || myformat == GIFFILE){
		int width, height, channels;
		unsigned char* image_data = stbi_load_from_memory(_img_data,_size, &width, &height, NULL, 4);
		channels = 4;
		Texture rettext = Renderer->load_texture_from_mem(image_data,width,height,channels,DkImageFormat_RGBA8_Unorm, 0);
		free(image_data); 
		return rettext;
	}
	if(myformat == JPGFILE){
		int width, height, channels;
		unsigned char* image_data = jpg_decode(_img_data,_size, &width, &height,&channels);
		Texture rettext =  Renderer->load_texture_from_mem(image_data,width,height,channels,DkImageFormat_RGBA8_Unorm, 0);
		free(image_data); 
		return rettext;
	}
	
	return {};
}

Texture CImgLoader::OpenRAWImageMemory(unsigned char *_img_data,int _w,int _h){
	
	Texture rettext =  Renderer->load_texture_from_mem(_img_data,_w,_h,4,DkImageFormat_RGBA8_Unorm, 0);
	return rettext;
	
}


Texture CImgLoader::OpenImageFile(std::string path){
	
	int myformat = guessImageFormat(path);
	if(myformat == -1){
		return {};
	}
	
	if(myformat == JPGFILE || myformat == PNGFILE || myformat == BMPFILE || myformat == GIFFILE){
		return Renderer->load_texture(path,DkImageFormat_RGBA8_Unorm, 0);
	} 
	
	return {};
}

