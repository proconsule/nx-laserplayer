#ifndef NXLASERPLAYER_UTILS_H
#define NXLASERPLAYER_UTILS_H

#include <string>
#include <filesystem>

enum class DiscType {
    UNKNOWN,
    DVD_VIDEO,
    BLU_RAY,
    VCD,
    SVCD
};


std::string DiscTypeToString(DiscType _type);
DiscType getDiscType(const std::string& directory_path);

bool pathExists(const std::string& path);
bool isDirectory(const std::string& path);
std::string joinPath(const std::string& base, const std::string& sub);

#endif /* UTILS_H */

