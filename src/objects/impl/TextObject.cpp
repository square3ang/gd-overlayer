#include "TextObject.hpp"


void TextObject::init()
{
    actualText = CCLabelBMFont::create(text.c_str(), fontName.c_str());
    actualText->setPosition(x, y);
    actualText->setRotation(rotation);
    actualText->setScale(fontSize / 24.0f);
    actualText->setColor(ccc3(color.r, color.g, color.b));
    actualText->setOpacity(color.a);
    CCDirector::sharedDirector()->getNotificationNode()->addChild(actualText);
}

void TextObject::update()
{
    if (actualText)
    {
        actualText->setPosition(x, y);
        actualText->setRotation(rotation);
        actualText->setScale(fontSize / 24.0f);
        actualText->setString(text.c_str());
        actualText->setFntFile(fontName.c_str());
        actualText->setColor(ccc3(color.r, color.g, color.b));
        actualText->setOpacity(color.a);
    }
}

void TextObject::drawSettings()
{
    static auto fonts = getEveryGDFont();

    auto valueChanged = false;
    valueChanged |= ImGui::DragFloat2("Position", &x);
    valueChanged |= ImGui::DragFloat("Rotation", &rotation);

    valueChanged |= ImGui::InputText("Text", &text);
    if (ImGui::BeginCombo("Font", fontName.c_str()))
    {
        for (const auto &fontname : fonts)
        {
            bool isSelected = (fontName == fontname);
            if (ImGui::Selectable(fontname.c_str(), isSelected))
            {
                fontName = fontname;
                valueChanged = true;
            }
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    valueChanged |= ImGui::DragFloat("Font Size", &fontSize, 0.25f, 1.0f, 100.0f);
    valueChanged |= byteColorEdit4("Color", color);

    if (valueChanged) update();
}

void TextObject::destroy()
{
    if (actualText)
    {
        actualText->removeFromParentAndCleanup(true);
        actualText = nullptr;
    }
}