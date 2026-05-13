#pragma once

#include "../Object.hpp"
#include "../../tag/TaggedText.hpp"
#include <Geode/Geode.hpp>
#include <imgui-cocos.hpp>
#include <string>

using namespace geode::prelude;

class TextObject : public Object {
public:
  CCTextAlignment alignment = kCCTextAlignmentLeft;
  std::string playingText = "Attempts: {Attempts}";
  std::string idleText = "";
  std::string fontName = "bigFont.fnt";
  float fontSize = 12.0f;
  ccColor4B color = {255, 255, 255, 255};
  CCLabelBMFont *actualText = nullptr;

  TextObject() {
    name = "Text Object";
    x = 200.0f;
    y = 200.0f;
    pivotX = 0.0f;
    pivotY = 1.0f;
  }

  void init() override;

  void update() override;
  void everyFrame() override;

  void drawSettings() override;

  void destroy() override;

  matjson::Value serialize() override;
  void deserialize(matjson::Value const &data) override;

private:
  TaggedText m_playingTagged;
  TaggedText m_idleTagged;
};