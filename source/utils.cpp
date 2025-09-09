#include "utils.h"

#include <string>
#include <cstdio>
#include <sys/stat.h>

#include <unistd.h>
#define PATH_SEPARATOR "/"



// Funzione helper per verificare se un percorso esiste
bool pathExists(const std::string& path) {
#ifdef _WIN32
    DWORD attributes = GetFileAttributesA(path.c_str());
    return (attributes != INVALID_FILE_ATTRIBUTES);
#else
    return (access(path.c_str(), F_OK) == 0);
#endif
}

// Funzione helper per verificare se un percorso è una directory
bool isDirectory(const std::string& path) {
    struct stat statbuf;
    if (stat(path.c_str(), &statbuf) != 0) {
        return false;
    }
    return S_ISDIR(statbuf.st_mode);
}

// Funzione helper per costruire un percorso
std::string joinPath(const std::string& base, const std::string& sub) {
    std::string result = base;
    
    // Rimuovi il separatore finale se presente
    if (!result.empty() && result.back() == '/') {
        result.pop_back();
    }
    
    // Aggiungi il separatore e la sottodirectory
    result += "/";
    result += sub;
    
    return result;
}

DiscType getDiscType(const std::string& directory_path) {
    
    printf("PATH: %s\r\n",directory_path.c_str());
    
    // Controlla per DVD (presenza della cartella VIDEO_TS)
    std::string video_ts_path = joinPath(directory_path, "VIDEO_TS");
    if (pathExists(video_ts_path) && isDirectory(video_ts_path)) {
        return DiscType::DVD_VIDEO;
    }
    
    // Controlla per Blu-ray (presenza della cartella BDMV)
    std::string bdmv_path = joinPath(directory_path, "BDMV");
    if (pathExists(bdmv_path) && isDirectory(bdmv_path)) {
        return DiscType::BLU_RAY;
    }
    
    // Controlla per VCD (presenza della cartella MPEGAV)
    std::string mpegav_path = joinPath(directory_path, "MPEGAV");
    if (pathExists(mpegav_path) && isDirectory(mpegav_path)) {
        return DiscType::VCD;
    }
    
    // Controlla per SVCD (presenza della cartella MPEG2)
    std::string mpeg2_path = joinPath(directory_path, "MPEG2");
    if (pathExists(mpeg2_path) && isDirectory(mpeg2_path)) {
        return DiscType::SVCD;
    }
    return DiscType::UNKNOWN;
}

std::string DiscTypeToString(DiscType _type) {
    switch (_type) {
        case DiscType::DVD_VIDEO:
            return "DVD-Video";
        case DiscType::BLU_RAY:
            return "Blu-ray";
        case DiscType::VCD:
            return "VCD";
        case DiscType::SVCD:
            return "SVCD";
        case DiscType::UNKNOWN:
            return "UNKNOWN";
    }
    return "UNKNOWN"; 
}