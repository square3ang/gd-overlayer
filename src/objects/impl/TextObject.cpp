#include "TextObject.hpp"
#include "../../Overlayer.hpp"
#include "../../imgui/imgui_stdlib.h"
#include "../../utils.hpp"

void TextObject::init() {
  m_actualText = CCLabelBMFont::create("", m_fontName.c_str());
  m_actualText->setPosition(m_position);
  m_actualText->setAnchorPoint(m_pivot);
  m_actualText->setAlignment(m_alignment);
  m_actualText->setRotation(m_rotation);
  m_actualText->setScaleX(m_scaleX);
  m_actualText->setScaleY(m_scaleY);
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
    m_actualText->setScaleX(m_scaleX);
    m_actualText->setScaleY(m_scaleY);
    m_actualText->setFntFile(m_fontName.c_str());
    m_actualText->setColor(ccc3(m_color.r, m_color.g, m_color.b));
    m_actualText->setOpacity(m_color.a);
  }
}
void TextObject::everyFrame() {
  if (m_actualText) {
    auto playLayer = PlayLayer::get();
    bool isPlaying = playLayer != nullptr;
    bool isSimulating = Overlayer::get()->m_simulationMode;

    if (isPlaying || isSimulating) {
      m_playingTagged.setRaw(m_playingText);
      m_playingTagged.apply(m_actualText, isPlaying, isSimulating);
    } else {
      m_idleTagged.setRaw(m_idleText);
      m_idleTagged.apply(m_actualText, isPlaying, isSimulating);
    }
  }
}

void TextObject::drawSettings() {
  static auto fonts = getEveryGDFont();

  auto valueChanged = false;
  valueChanged |= ImGui::DragFloat2("Position", &m_position.x);
  valueChanged |= ImGui::DragFloat2("Pivot", &m_pivot.x, 0.01f, 0.0f, 1.0f);

  float oldX = m_scaleX;
  float oldY = m_scaleY;

  float startX = ImGui::GetCursorPosX();
  float itemWidth = ImGui::CalcItemWidth();
  float innerSpacing = ImGui::GetStyle().ItemInnerSpacing.x;
  float checkboxWidth = ImGui::GetFrameHeight();
  float inputWidth = (itemWidth - checkboxWidth - innerSpacing * 2) / 2.0f;

  ImGui::PushItemWidth(inputWidth);
  if (ImGui::DragFloat("##X", &m_scaleX, 0.005f, 0.0f, 100.0f)) {
    if (m_linkScale && oldX != 0) {
      m_scaleY = (m_scaleX / oldX) * oldY;
    }
    valueChanged = true;
  }
  ImGui::PopItemWidth();

  ImGui::SameLine(0, innerSpacing);
  valueChanged |= ImGui::Checkbox("##Link", &m_linkScale);

  ImGui::SameLine(0, innerSpacing);
  ImGui::PushItemWidth(inputWidth);
  if (ImGui::DragFloat("##Y", &m_scaleY, 0.005f, 0.0f, 100.0f)) {
    if (m_linkScale && oldY != 0) {
      m_scaleX = (m_scaleY / oldY) * oldX;
    }
    valueChanged = true;
  }
  ImGui::PopItemWidth();

  ImGui::SameLine(startX + itemWidth + innerSpacing);
  ImGui::Text("Scale");
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
  auto data = Object::serialize();
  data["type"] = "text";
  data["alignment"] = static_cast<int>(m_alignment);
  data["playingText"] = m_playingText;
  data["idleText"] = m_idleText;
  data["fontName"] = m_fontName;
  data["color"] = m_color;
  return data;
}

void TextObject::deserialize(matjson::Value const &data) {
  Object::deserialize(data);

  if (data.contains("fontSize") && !data.contains("scaleX")) {
    float fontSize = data["fontSize"].asDouble().unwrapOr(24.0f);
    m_scaleX = m_scaleY = fontSize / 24.0f;
  }

  m_alignment = static_cast<CCTextAlignment>(
      data["alignment"].asInt().unwrapOr(m_alignment));

  if (data.contains("text")) {
    m_playingText = data["text"].asString().unwrapOr(m_playingText);
    m_idleText = m_playingText;
  }

  m_playingText = data["playingText"].asString().unwrapOr(m_playingText);
  m_idleText = data["idleText"].asString().unwrapOr(m_idleText);
  m_fontName = data["fontName"].asString().unwrapOr(m_fontName);
  m_color = data["color"].as<ccColor4B>().unwrapOr(m_color);
}