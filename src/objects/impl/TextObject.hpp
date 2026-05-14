#pragma once

#include "../../tag/TaggedText.hpp"
#include "../Object.hpp"
#include <Geode/Geode.hpp>
#include <imgui-cocos.hpp>
#include <string>


using namespace geode::prelude;

class TextObject : public Object {
public:
  CCTextAlignment m_alignment = kCCTextAlignmentLeft;
  std::string m_playingText = "Attempts: {Attempts}";
  std::string m_idleText = "";
  std::string m_fontName = "bigFont.fnt";
  float m_fontSize = 12.0f;
  ccColor4B m_color = {255, 255, 255, 255};
  CCLabelBMFont *m_actualText = nullptr;

  TextObject() {
    m_name = "Text Object";
    m_position = ccp(200.0f, 200.0f);
    m_pivot = ccp(0.0f, 1.0f);
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