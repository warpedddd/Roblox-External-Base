#include <ShlObj.h>
#include <filesystem>
#include <fstream>
#include <string>
#include "logs.hpp"

std::uint64_t Logs::GetRenderView() // shoutout to whoever who found this method
{
    wchar_t path[MAX_PATH];

    if (FAILED(SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path)))
        return 0;

    std::wstring LogPath = std::wstring(path) + L"\\Roblox\\logs";

    std::vector<std::filesystem::path> FoundPath;
    for (const auto& Entry : std::filesystem::directory_iterator(LogPath)) {
        if (Entry.path().extension() == ".log" != std::string::npos) {
            FoundPath.push_back(Entry.path());
        }
    }

    if (FoundPath.empty()) 
        return 0;

    std::sort(FoundPath.begin(), FoundPath.end(), [](const std::filesystem::path& a, const std::filesystem::path& b) { return std::filesystem::last_write_time(a) > std::filesystem::last_write_time(b); });

    std::ifstream File(FoundPath[0]);
    std::string Line;
    while (std::getline(File, Line)) {
        if (Line.find("initialize view(") != std::string::npos) {
            std::string Data = Line.substr(Line.find("initialize view(") + 17);

            Data = Data.substr(0, Data.find(')'));
            return std::strtoull(Data.c_str(), nullptr, 16);
        }
    }

    return false;
}