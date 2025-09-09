#ifndef BLURAYNAV_H
#define BLURAYNAV_H

#include <string>
#include <libbluray/bluray.h>

#include <vector>
#include "discinfo_common.h"


class CBLURAYNAV:public CDISCINFO{
public:
    CBLURAYNAV(std::string _path): CDISCINFO(_path){};
    ~CBLURAYNAV();
    void BDInfo();
    
    
    //std::vector<titles_info_struct> titles;
};

#endif /* BLURAYNAV_H */

