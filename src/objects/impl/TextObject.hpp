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
    std::string text = "Hello, World!";
    std::string fontName = "bigFont.fnt";
    float fontSize = 12.0f;
    ccColor4B color = { 255, 255, 255, 255 };
    CCLabelBMFont *actualText = nullptr;

    TextObject() {
        name = "Text Object";
        x = 200.0f;
        y = 200.0f;
    }

    void init() override;

    void update() override;

    void drawSettings() override;

    void destroy() override;
};