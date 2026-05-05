#pragma once

#include "../Object.hpp"
#include <string>
#include <Geode/Geode.hpp>
#include <imgui-cocos.hpp>
#include "../../imgui/imgui_stdlib.h"
#include "../../utils.hpp"

using namespace geode::prelude;

class TextObject : public Object
{
public:
    std::string text;
    std::string fontName;
    float fontSize;
    ccColor4B color;
    CCLabelBMFont *actualText;

    void init() override;

    void update() override;

    void drawSettings() override;

    ~TextObject();
};