#pragma once

#include <Geode/Geode.hpp>
#include <imgui-cocos.hpp>
#include <Geode/loader/Mod.hpp>
#include <filesystem>
#include <set>
#include <string>
#include <vector>

using namespace geode::prelude;
namespace fs = std::filesystem;


inline bool byteColorEdit4(const char *label, ccColor4B &color)
{
    float proxy[4] = {
        color.r / 255.0f,
        color.g / 255.0f,
        color.b / 255.0f,
        color.a / 255.0f};

    if (ImGui::ColorEdit4(label, proxy))
    {
        color.r = static_cast<unsigned char>(proxy[0] * 255.0f);
        color.g = static_cast<unsigned char>(proxy[1] * 255.0f);
        color.b = static_cast<unsigned char>(proxy[2] * 255.0f);
        color.a = static_cast<unsigned char>(proxy[3] * 255.0f);
        return true;
    }
    return false;
}

inline std::vector<std::string> getEveryGDFont() {
    std::set<std::string> uniqueFonts;
    auto searchPaths = CCFileUtils::sharedFileUtils()->getSearchPaths();

    for (const auto& path : searchPaths) {
        fs::path fsPath(path);
        if (fs::exists(fsPath) && fs::is_directory(fsPath)) {
            for (const auto& entry : fs::directory_iterator(fsPath)) {
                if (entry.path().extension() == ".fnt") {
                    uniqueFonts.insert(entry.path().filename().string());
                }
            }
        }
    }
    return std::vector<std::string>(uniqueFonts.begin(), uniqueFonts.end());
}