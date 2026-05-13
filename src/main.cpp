#include <Geode/Geode.hpp>
#include <imgui-cocos.hpp>
#include <vector>

#include "imgui/imgui_stdlib.h"
#include "objects/Object.hpp"
#include "objects/impl/TextObject.hpp"
#include "tag/TagRegistry.hpp"

using namespace geode::prelude;

static std::vector<Object *> objects;
static bool settingsOpen = false;

void saveToCustomFile() {
  auto path = Mod::get()->getSaveDir() / "objects_data.json";

  matjson::Value data = matjson::Value::array();

  for (auto &obj : objects) {
    data.push(obj->serialize());
  }

  if (!utils::file::writeToJson(path, data).isOk()) {
    log::error("Failed to save objects: {}", path);
  }
}

void loadFromCustomFile() {
  auto path = Mod::get()->getSaveDir() / "objects_data.json";
  auto __data = utils::file::readFromJson<matjson::Value>(path);
  if (!__data.isOk())
    return;
  auto _data = __data.unwrap().asArray();
  if (!_data.isOk())
    return;
  auto data = _data.unwrap();

  for (auto obj : data) {
    auto type = obj["type"].asString().unwrapOr("unknown");
    if (type == "text") {
      auto textObject = new TextObject();
      textObject->deserialize(obj);
      textObject->init();
      objects.push_back(textObject);
    }
  }
}

#include <Geode/modify/AppDelegate.hpp>
class $modify(MyAppDelegate, AppDelegate) {
  void trySaveGame(bool force) {
    AppDelegate::trySaveGame(force);
    saveToCustomFile();
  }
};

#include <Geode/modify/CCDirector.hpp>
class $modify(MyCCDirector, CCDirector) {
  void drawScene() {
    CCDirector::drawScene();
    for (auto &obj : objects) {
      obj->everyFrame();
    }
  }

  void willSwitchToScene(CCScene *scene) {
    CCDirector::willSwitchToScene(scene);
    TagRegistry::get().clearCache();
  }
};

void registerTags() {
  auto attemptsHandler = [](std::string_view arg) -> TagValue {
    auto playLayer = PlayLayer::get();
    if (!playLayer)
      return 0;

    if (arg == "total") {
      return playLayer->m_level->m_attempts;
    }

    return playLayer->m_attempts;
  };
  TagRegistry::get().registerTag("Attempts", attemptsHandler, false);
}

void renderGUI() {
  if (!settingsOpen)
    return;
  static auto io = ImGui::GetIO();
  ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x / 5, io.DisplaySize.y / 2));
  ImGui::Begin("Overlayer", &settingsOpen,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);

  if (ImGui::Button("Add Text")) {
    auto textObject = new TextObject();
    textObject->init();
    objects.push_back(textObject);
  }

  for (auto &obj : objects) {
    ImGui::BeginGroup();
    ImGui::InputText(fmt::format("##Name_{}", obj->uuid).c_str(), &obj->name);
    ImGui::SameLine();
    if (ImGui::Button(fmt::format("Edit##{}_EDIT", obj->uuid).c_str())) {
      obj->settingsOpen = !obj->settingsOpen;
    }
    ImGui::SameLine();
    if (ImGui::Button(fmt::format("Delete##{}_DELETE", obj->uuid).c_str())) {
      // Remove the object from the list and delete it
      auto it = std::find(objects.begin(), objects.end(), obj);
      if (it != objects.end()) {
        (*it)->destroy();
        delete *it;
        objects.erase(it);
      }
    }
    ImGui::EndGroup();
  }

  ImGui::End();

  for (auto &obj : objects) {
    if (obj->settingsOpen) {
      ImGui::SetNextWindowSize(
          ImVec2(io.DisplaySize.x / 5, io.DisplaySize.y / 2), ImGuiCond_Once);
      ImGui::SetNextWindowPos(
          ImVec2(io.DisplaySize.x / 2, io.DisplaySize.y / 2), ImGuiCond_Once,
          ImVec2(0.5f, 0.5f));
      ImGui::Begin(
          fmt::format("{} Settings##{}_SETTINGS", obj->name, obj->uuid).c_str(),
          &obj->settingsOpen, ImGuiWindowFlags_NoSavedSettings);

      obj->drawSettings();

      ImGui::End();
    }
  }
}

void setupGUI() {
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
      .draw(renderGUI);
}

$on_mod(Loaded) {

  listenForKeybindSettingPresses(
      "toggle-menu",
      [](Keybind const &keybind, bool down, bool repeat, double timestamp) {
        if (down)
          settingsOpen = !settingsOpen;
      });

  loadFromCustomFile();
  registerTags();
  setupGUI();
}