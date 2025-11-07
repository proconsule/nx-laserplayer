#ifndef NXLASERPLAYER_UTILS_H
#define NXLASERPLAYER_UTILS_H

#include <string>
#include <filesystem>
#include <vector>

enum class DiscType {
    UNKNOWN,
    DVD_VIDEO,
    BLU_RAY,
    VCD,
    SVCD
};


std::string DiscTypeToString(DiscType _type);
DiscType getDiscType(const std::string& directory_path);

extern std::vector<std::string> dvd_audio_languages;
extern std::vector<std::string> dvd_audio_languages_extended;

int getlnagIDX(std::string _lang);
std::string millisecondsToTimeStringFast(long long milliseconds);

#endif /* UTILS_H */

