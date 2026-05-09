#include "TextObject.hpp"
#include "../../imgui/imgui_stdlib.h"
#include "../../utils.hpp"

void TextObject::init() {
  actualText = CCLabelBMFont::create(text.c_str(), fontName.c_str());
  actualText->setPosition(x, y);
  actualText->setAnchorPoint(ccp(pivotX, pivotY));
  actualText->setAlignment(alignment);
  actualText->setRotation(rotation);
  actualText->setScale(fontSize / 24.0f);
  actualText->setColor(ccc3(color.r, color.g, color.b));
  actualText->setOpacity(color.a);
  CCDirector::sharedDirector()->getNotificationNode()->addChild(actualText);
}

void TextObject::update() {
  if (actualText) {
    actualText->setPosition(x, y);
    actualText->setAnchorPoint(ccp(pivotX, pivotY));
    actualText->setAlignment(alignment);
    actualText->setRotation(rotation);
    actualText->setScale(fontSize / 24.0f);
    actualText->setString(text.c_str());
    actualText->setFntFile(fontName.c_str());
    actualText->setColor(ccc3(color.r, color.g, color.b));
    actualText->setOpacity(color.a);
  }
}

void TextObject::drawSettings() {
  static auto fonts = getEveryGDFont();

  auto valueChanged = false;
  valueChanged |= ImGui::DragFloat2("Position", &x);
  valueChanged |= ImGui::DragFloat2("Pivot", &pivotX, 0.01f, 0.0f, 1.0f);
  valueChanged |= ImGui::DragFloat("Rotation", &rotation);
  if (ImGui::BeginCombo("Font", fontName.c_str())) {
    for (const auto &fontname : fonts) {
      bool isSelected = (fontName == fontname);
      if (ImGui::Selectable(fontname.c_str(), isSelected)) {
        fontName = fontname;
        valueChanged = true;
      }
      if (isSelected)
        ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }
  valueChanged |=
      ImGui::Combo("Alignment", (int *)&alignment, "Left\0Center\0Right\0");
  valueChanged |= ImGui::DragFloat("Font Size", &fontSize, 0.25f, 1.0f, 100.0f);
  valueChanged |= byteColorEdit4("Color", color);
  valueChanged |= ImGui::InputTextMultiline("Text", &text);

  if (valueChanged)
    update();
}

void TextObject::destroy() {
  if (actualText) {
    actualText->removeFromParentAndCleanup(true);
    actualText = nullptr;
  }
}

matjson::Value TextObject::serialize() {
  return matjson::makeObject({
      {"type", "text"},
      {"name", name},
      {"x", x},
      {"y", y},
      {"pivotX", pivotX},
      {"pivotY", pivotY},
      {"alignment", static_cast<int>(alignment)},
      {"rotation", rotation},
      {"text", text},
      {"fontName", fontName},
      {"fontSize", fontSize},
      {"color", color},
  });
}

void TextObject::deserialize(matjson::Value const &data) {
  name = data["name"].asString().unwrapOr(name);
  x = data["x"].asDouble().unwrapOr(x);
  y = data["y"].asDouble().unwrapOr(y);
  pivotX = data["pivotX"].asDouble().unwrapOr(pivotX);
  pivotY = data["pivotY"].asDouble().unwrapOr(pivotY);
  alignment = static_cast<CCTextAlignment>(
      data["alignment"].asInt().unwrapOr(alignment));
  rotation = data["rotation"].asDouble().unwrapOr(rotation);
  text = data["text"].asString().unwrapOr(text);
  fontName = data["fontName"].asString().unwrapOr(fontName);
  fontSize = data["fontSize"].asDouble().unwrapOr(fontSize);
  color = data["color"].as<ccColor4B>().unwrapOr(color);
}