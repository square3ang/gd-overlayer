#include "TextObject.hpp"
#include "../../imgui/imgui_stdlib.h"
#include "../../tag/TagRegistry.hpp"
#include "../../utils.hpp"

void TextObject::init() {
  m_actualText = CCLabelBMFont::create("", m_fontName.c_str());
  m_actualText->setPosition(m_position);
  m_actualText->setAnchorPoint(m_pivot);
  m_actualText->setAlignment(m_alignment);
  m_actualText->setRotation(m_rotation);
  m_actualText->setScale(m_fontSize / 24.0f);
  m_actualText->setColor(ccc3(m_color.r, m_color.g, m_color.b));
  m_actualText->setOpacity(m_color.a);
  CCDirector::sharedDirector()->getNotificationNode()->addChild(m_actualText);
}

void TextObject::update() {
  if (m_actualText) {
    m_actualText->setPosition(m_position);
    m_actualText->setAnchorPoint(m_pivot);
    m_actualText->setAlignment(m_alignment);
    m_actualText->setRotation(m_rotation);
    m_actualText->setScale(m_fontSize / 24.0f);
    m_actualText->setFntFile(m_fontName.c_str());
    m_actualText->setColor(ccc3(m_color.r, m_color.g, m_color.b));
    m_actualText->setOpacity(m_color.a);
  }
}
void TextObject::everyFrame() {
  if (m_actualText) {
    auto playLayer = PlayLayer::get();
    bool isPlaying = playLayer != nullptr;
    bool isSimulating = TagRegistry::get().m_simulationMode;

    if (isPlaying || isSimulating) {
      m_playingTagged.setRaw(m_playingText);
      m_playingTagged.apply(m_actualText, isPlaying);
    } else {
      m_idleTagged.setRaw(m_idleText);
      m_idleTagged.apply(m_actualText, isPlaying);
    }
  }
}

void TextObject::drawSettings() {
  static auto fonts = getEveryGDFont();

  auto valueChanged = false;
  valueChanged |= ImGui::DragFloat2("Position", &m_position.x);
  valueChanged |= ImGui::DragFloat2("Pivot", &m_pivot.x, 0.01f, 0.0f, 1.0f);
  valueChanged |= ImGui::DragFloat("Rotation", &m_rotation);
  if (ImGui::BeginCombo("Font", m_fontName.c_str())) {
    for (const auto &fontname : fonts) {
      bool isSelected = (m_fontName == fontname);
      if (ImGui::Selectable(fontname.c_str(), isSelected)) {
        m_fontName = fontname;
        valueChanged = true;
      }
      if (isSelected)
        ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }
  valueChanged |=
      ImGui::Combo("Alignment", (int *)&m_alignment, "Left\0Center\0Right\0");
  valueChanged |=
      ImGui::DragFloat("Font Size", &m_fontSize, 0.25f, 1.0f, 1000.0f);
  valueChanged |= byteColorEdit4("Color", m_color);

  ImGui::Text("Text Templates:");
  ImGui::InputTextMultiline("Playing Text", &m_playingText);
  ImGui::InputTextMultiline("Idle Text", &m_idleText);

  if (valueChanged)
    update();
}

void TextObject::destroy() {
  if (m_actualText) {
    m_actualText->removeFromParentAndCleanup(true);
    m_actualText = nullptr;
  }
}

matjson::Value TextObject::serialize() {
  return matjson::makeObject({
      {"type", "text"},
      {"name", m_name},
      {"x", m_position.x},
      {"y", m_position.y},
      {"pivotX", m_pivot.x},
      {"pivotY", m_pivot.y},
      {"alignment", static_cast<int>(m_alignment)},
      {"rotation", m_rotation},
      {"playingText", m_playingText},
      {"idleText", m_idleText},
      {"fontName", m_fontName},
      {"fontSize", m_fontSize},
      {"color", m_color},
  });
}

void TextObject::deserialize(matjson::Value const &data) {
  m_name = data["name"].asString().unwrapOr(m_name);
  m_position.x = data["x"].asDouble().unwrapOr(m_position.x);
  m_position.y = data["y"].asDouble().unwrapOr(m_position.y);
  m_pivot.x = data["pivotX"].asDouble().unwrapOr(m_pivot.x);
  m_pivot.y = data["pivotY"].asDouble().unwrapOr(m_pivot.y);
  m_alignment = static_cast<CCTextAlignment>(
      data["alignment"].asInt().unwrapOr(m_alignment));
  m_rotation = data["rotation"].asDouble().unwrapOr(m_rotation);

  // very old ver compat
  if (data.contains("text")) {
    m_playingText = data["text"].asString().unwrapOr(m_playingText);
    m_idleText = m_playingText;
  }

  m_playingText = data["playingText"].asString().unwrapOr(m_playingText);
  m_idleText = data["idleText"].asString().unwrapOr(m_idleText);
  m_fontName = data["fontName"].asString().unwrapOr(m_fontName);
  m_fontSize = data["fontSize"].asDouble().unwrapOr(m_fontSize);
  m_color = data["color"].as<ccColor4B>().unwrapOr(m_color);
}