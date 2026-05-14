#include "Overlayer.hpp"
#include "imgui/imgui_stdlib.h"
#include "objects/impl/TextObject.hpp"
#include "tag/TagRegistry.hpp"
#include <Geode/Geode.hpp>
#include <imgui-cocos.hpp>

using namespace geode::prelude;

Overlayer *Overlayer::get() {
  static Overlayer instance;
  return &instance;
}

void Overlayer::save() {
  auto path = Mod::get()->getSaveDir() / "objects_data.json";

  matjson::Value data = matjson::Value::array();

  for (auto &obj : m_objects) {
    data.push(obj->serialize());
  }

  if (!utils::file::writeToJson(path, data)) {
    log::error("Failed to save objects: {}", path);
  }
}

void Overlayer::load() {
  auto path = Mod::get()->getSaveDir() / "objects_data.json";
  auto __data = utils::file::readFromJson<matjson::Value>(path);
  if (!__data)
    return;
  auto _data = __data.unwrap().asArray();
  if (!_data)
    return;
  auto data = _data.unwrap();

  for (auto obj : data) {
    auto type = obj["type"].asString().unwrapOr("unknown");
    if (type == "text") {
      auto textObject = std::make_unique<TextObject>();
      textObject->deserialize(obj);
      textObject->init();
      m_objects.push_back(std::move(textObject));
    }
  }
}

#include "tag/impl/GamePlay.hpp"
void Overlayer::registerTags() { GamePlay::registerTags(); }

void Overlayer::renderGUI() {
  if (!m_settingsOpen)
    return;
  static auto io = ImGui::GetIO();
  ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x / 5, io.DisplaySize.y / 2),
                           ImGuiCond_Once);
  ImGui::Begin("Overlayer", &m_settingsOpen, ImGuiWindowFlags_NoSavedSettings);

  auto &reg = TagRegistry::get();
  ImGui::Checkbox("Simulate In-game", &m_simulationMode);

  if (ImGui::Button("Add Text")) {
    m_objects.push_back(std::make_unique<TextObject>());
    m_objects.back()->init();
  }

  for (auto &obj : m_objects) {
    ImGui::BeginGroup();
    ImGui::InputText(fmt::format("##Name_{}", obj->m_uuid).c_str(), &obj->m_name);
    ImGui::SameLine();
    if (ImGui::Button(fmt::format("Edit##{}_EDIT", obj->m_uuid).c_str())) {
      obj->m_settingsOpen = !obj->m_settingsOpen;
    }
    ImGui::SameLine();
    if (ImGui::Button(fmt::format("Delete##{}_DELETE", obj->m_uuid).c_str())) {
      auto it = std::find(m_objects.begin(), m_objects.end(), obj);
      if (it != m_objects.end()) {
        (*it)->destroy();
        m_objects.erase(it);
        break;
      }
    }
    ImGui::EndGroup();
  }

  ImGui::End();

  for (auto &obj : m_objects) {
    if (obj->m_settingsOpen) {
      ImGui::SetNextWindowSize(
          ImVec2(io.DisplaySize.x / 5, io.DisplaySize.y / 2), ImGuiCond_Once);
      ImGui::SetNextWindowPos(
          ImVec2(io.DisplaySize.x / 2, io.DisplaySize.y / 2), ImGuiCond_Once,
          ImVec2(0.5f, 0.5f));
      ImGui::Begin(
          fmt::format("{} Settings##{}_SETTINGS", obj->m_name, obj->m_uuid).c_str(),
          &obj->m_settingsOpen, ImGuiWindowFlags_NoSavedSettings);

      obj->drawSettings();

      ImGui::End();
    }
  }
}

void Overlayer::setupGUI() {
  ImGuiCocos::get()
      .setup([] {
        auto io = ImGui::GetIO();
        // io.IniFilename = nullptr; not works
        auto *font = ImGui::GetIO().Fonts->AddFontFromFileTTF(
            (Mod::get()->getResourcesDir() / "PretendardVariable.ttf")
                .string()
                .c_str(),
            18.0f);
        io.FontDefault = font;
        io.Fonts->Build();

        auto style = &ImGui::GetStyle();
        style->Alpha = 1.0f;
        style->WindowBorderSize = 0.0f;
        style->Colors[ImGuiCol_TitleBg] = ImVec4{0.25, 0.25, 1.0, 1.0};
        style->Colors[ImGuiCol_TitleBgActive] = ImVec4{0.25, 0.25, 1.0, 1.0};
      })
      .draw([this] { renderGUI(); });
}
