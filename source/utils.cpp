#include "utils.h"
#include <sys/stat.h>

#include <unistd.h>
#include <algorithm>

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
    return "Sconosciuto"; // Ritorno di sicurezza
}

std::vector<std::string> dvd_audio_languages = { "",
    "aa", "ab", "ae", "af", "ak", "am", "an", "ar", "as", "av", "ay", "az",
    "ba", "be", "bg", "bh", "bi", "bm", "bn", "bo", "br", "bs", "ca", "ce",
    "ch", "co", "cr", "cs", "cu", "cv", "cy", "da", "de", "dv", "dz", "ee",
    "el", "en", "eo", "es", "et", "eu", "fa", "ff", "fi", "fj", "fo", "fr",
    "fy", "ga", "gd", "gl", "gn", "gu", "gv", "ha", "he", "hi", "ho", "hr",
    "ht", "hu", "hy", "hz", "ia", "id", "ie", "ig", "ii", "ik", "io", "is",
    "it", "iu", "ja", "jv", "ka", "kg", "ki", "kj", "kk", "kl", "km", "kn",
    "ko", "kr", "ks", "ku", "kv", "kw", "ky", "la", "lb", "lg", "li", "ln",
    "lo", "lt", "lu", "lv", "mg", "mh", "mi", "mk", "ml", "mn", "mo", "mr",
    "ms", "mt", "my", "na", "nb", "nd", "ne", "ng", "nl", "nn", "no", "nr",
    "nv", "ny", "oc", "oj", "om", "or", "os", "pa", "pi", "pl", "ps", "pt",
    "qu", "rm", "rn", "ro", "ru", "rw", "sa", "sc", "sd", "se", "sg", "si",
    "sk", "sl", "sm", "sn", "so", "sq", "sr", "ss", "st", "su", "sv", "sw",
    "ta", "te", "tg", "th", "ti", "tk", "tl", "tn", "to", "tr", "ts", "tt",
    "tw", "ty", "ug", "uk", "ur", "uz", "ve", "vi", "vo", "wa", "wo", "xh",
    "yi", "yo", "za", "zh", "zu"
};

std::vector<std::string> dvd_audio_languages_extended = { "Content Default",
    "Afar", "Abkhazian", "Avestan", "Afrikaans", "Akan", "Amharic", "Aragonese",
    "Arabic", "Assamese", "Avaric", "Aymara", "Azerbaijani", "Bashkir",
    "Belarusian", "Bulgarian", "Bihari languages", "Bislama", "Bambara",
    "Bengali", "Tibetan", "Breton", "Bosnian", "Catalan", "Chechen", "Chamorro",
    "Corsican", "Cree", "Czech", "Church Slavic", "Chuvash", "Welsh", "Danish",
    "German", "Divehi", "Dzongkha", "Ewe", "Greek", "English", "Esperanto",
    "Spanish", "Estonian", "Basque", "Persian", "Fulah", "Finnish", "Fijian",
    "Faroese", "French", "Western Frisian", "Irish", "Gaelic", "Galician",
    "Guarani", "Gujarati", "Manx", "Hausa", "Hebrew", "Hindi", "Hiri Motu",
    "Croatian", "Haitian", "Hungarian", "Armenian", "Herero", "Interlingua",
    "Indonesian", "Interlingue", "Igbo", "Sichuan Yi", "Inupiaq", "Ido",
    "Icelandic", "Italian", "Inuktitut", "Japanese", "Javanese", "Georgian",
    "Kongo", "Kikuyu", "Kuanyama", "Kazakh", "Kalaallisut", "Cambodian",
    "Kannada", "Korean", "Kanuri", "Kashmiri", "Kurdish", "Komi", "Cornish",
    "Kirghiz", "Latin", "Luxembourgish", "Ganda", "Limburgan", "Lingala", "Lao",
    "Lithuanian", "Luba-Katanga", "Latvian", "Malagasy", "Marshallese", "Maori",
    "Macedonian", "Malayalam", "Mongolian", "Moldavian", "Marathi", "Malay",
    "Maltese", "Burmese", "Nauru", "Norwegian Bokmål", "Ndebele, North",
    "Nepali", "Ndonga", "Dutch", "Norwegian Nynorsk", "Norwegian", "Ndebele, South",
    "Navajo", "Chichewa", "Occitan", "Ojibwa", "Oromo", "Oriya", "Ossetian",
    "Panjabi", "Pali", "Polish", "Pushto", "Portuguese", "Quechua", "Romansh",
    "Romanian", "Rundi", "Russian", "Kinyarwanda", "Sanskrit", "Sardinian",
    "Sindhi", "Northern Sami", "Sango", "Serbo-Croatian", "Sinhalese", "Slovak",
    "Slovenian", "Samoan", "Shona", "Somali", "Albanian", "Serbian", "Swati",
    "Sotho, Southern", "Sundanese", "Swedish", "Swahili", "Tamil", "Telugu",
    "Tajik", "Thai", "Tigrinya", "Turkmen", "Tagalog", "Tswana", "Tonga",
    "Turkish", "Tsonga", "Tatar", "Twi", "Tahitian", "Uighur", "Ukrainian",
    "Urdu", "Uzbek", "Venda", "Vietnamese", "Volapük", "Walloon", "Wolof",
    "Xhosa", "Yiddish", "Yoruba", "Zhuang", "Chinese", "Zulu"
};

int getlnagIDX(std::string _lang){
    
    auto it = std::find(dvd_audio_languages.begin(), dvd_audio_languages.end(), _lang);

    if (it != dvd_audio_languages.end()) {
        auto index = std::distance(dvd_audio_languages.begin(), it);
        return index;
    } 
    return -1;
}

std::string millisecondsToTimeStringFast(long long milliseconds) {
    long long totalSeconds = milliseconds / 1000;
    
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;
    
    std::string result;
    result.reserve(8); // "HH:MM:SS" = 8 caratteri
    
    // Ore
    result += (hours < 10) ? "0" : "";
    result += std::to_string(hours);
    result += ":";
    
    // Minuti
    result += (minutes < 10) ? "0" : "";
    result += std::to_string(minutes);
    result += ":";
    
    // Secondi
    result += (seconds < 10) ? "0" : "";
    result += std::to_string(seconds);
    
    return result;
}