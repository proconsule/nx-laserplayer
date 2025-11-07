#ifndef SVCDNAV_H
#define SVCDNAV_H

#include <string>
#include <libbluray/bluray.h>

#include <vector>
#include "discinfo_common.h"


class CSVCDNAV:public CDISCINFO{
public:
    CSVCDNAV(std::string _path): CDISCINFO(_path){};
    ~CSVCDNAV();
    void SVCDInfo();
    void VCDInfo();

};


#endif /* SVCDNAV_H */

