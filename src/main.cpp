#include <Geode/Geode.hpp>
#include <imgui-cocos.hpp>
#include <vector>

#include "imgui/imgui_stdlib.h"
#include "objects/Object.hpp"
#include "objects/impl/TextObject.hpp"

using namespace geode::prelude;

static std::vector<Object *> objects;
static bool settingsOpen = false;

$on_mod(Loaded) {

  listenForKeybindSettingPresses(
      "toggle-menu",
      [](Keybind const &keybind, bool down, bool repeat, double timestamp) {
        if (down)
          settingsOpen = !settingsOpen;
      });

  ImGuiCocos::get()
      .setup([] {
        auto io = ImGui::GetIO();
        io.IniFilename = nullptr;
        auto *font = ImGui::GetIO().Fonts->AddFontFromFileTTF(
            (Mod::get()->getResourcesDir() / "PretendardVariable.ttf")
                .string()
                .c_str(),
            18.0f);
        io.FontDefault = font;
        io.Fonts->Build();

        auto style = &ImGui::GetStyle();
        style->WindowRounding = 10.0f;
        style->WindowPadding = ImVec2(10.0f, 10.0f);
        style->FrameRounding = 5.0f;
        style->Alpha = 1.0f;

        auto hexToImVec4 = [](const char *hex, float alpha = 1.0f) -> ImVec4 {
          int r, g, b;
          if (sscanf(hex, "#%02x%02x%02x", &r, &g, &b) != 3)
            return ImVec4(1, 1, 1, alpha);
          return ImVec4(r / 255.0f, g / 255.0f, b / 255.0f, alpha);
        };

        // AI is optimized to do this job lol

        auto &colors = style->Colors;
        colors[ImGuiCol_Text] = hexToImVec4("#e4e4e6");
        colors[ImGuiCol_TextDisabled] = hexToImVec4("#aaa7b3");
        colors[ImGuiCol_WindowBg] = hexToImVec4("#323233");
        colors[ImGuiCol_ChildBg] = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_PopupBg] = hexToImVec4("#323233", 0.95f);
        colors[ImGuiCol_Border] = hexToImVec4("#aaa7b3", 0.5f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg] = hexToImVec4("#403266");
        colors[ImGuiCol_FrameBgHovered] = hexToImVec4("#595366");
        colors[ImGuiCol_FrameBgActive] = hexToImVec4("#C0B1E6");
        colors[ImGuiCol_TitleBg] = hexToImVec4("#5e5c66");
        colors[ImGuiCol_TitleBgActive] = hexToImVec4("#403266");
        colors[ImGuiCol_TitleBgCollapsed] = hexToImVec4("#323233", 0.5f);
        colors[ImGuiCol_MenuBarBg] = hexToImVec4("#323233");
        colors[ImGuiCol_ScrollbarBg] = hexToImVec4("#323233", 0.5f);
        colors[ImGuiCol_ScrollbarGrab] = hexToImVec4("#aaa7b3");
        colors[ImGuiCol_ScrollbarGrabHovered] = hexToImVec4("#dedbe6");
        colors[ImGuiCol_ScrollbarGrabActive] = hexToImVec4("#C0B1E6");
        colors[ImGuiCol_CheckMark] = hexToImVec4("#C0B1E6");
        colors[ImGuiCol_SliderGrab] = hexToImVec4("#C0B1E6");
        colors[ImGuiCol_SliderGrabActive] = hexToImVec4("#CBC0E6");
        colors[ImGuiCol_Button] = hexToImVec4("#403266");
        colors[ImGuiCol_ButtonHovered] = hexToImVec4("#595366");
        colors[ImGuiCol_ButtonActive] = hexToImVec4("#C0B1E6");
        colors[ImGuiCol_Header] = hexToImVec4("#403266");
        colors[ImGuiCol_HeaderHovered] = hexToImVec4("#595366");
        colors[ImGuiCol_HeaderActive] = hexToImVec4("#C0B1E6");
        colors[ImGuiCol_Separator] = hexToImVec4("#aaa7b3", 0.5f);
        colors[ImGuiCol_SeparatorHovered] = hexToImVec4("#595366");
        colors[ImGuiCol_SeparatorActive] = hexToImVec4("#C0B1E6");
        colors[ImGuiCol_ResizeGrip] = hexToImVec4("#403266", 0.2f);
        colors[ImGuiCol_ResizeGripHovered] = hexToImVec4("#595366", 0.67f);
        colors[ImGuiCol_ResizeGripActive] = hexToImVec4("#C0B1E6", 0.95f);
        colors[ImGuiCol_Tab] = hexToImVec4("#403266", 0.86f);
        colors[ImGuiCol_TabHovered] = hexToImVec4("#595366");
        colors[ImGuiCol_TabActive] = hexToImVec4("#C0B1E6");
        colors[ImGuiCol_TabUnfocused] = hexToImVec4("#323233");
        colors[ImGuiCol_TabUnfocusedActive] = hexToImVec4("#595366");
        colors[ImGuiCol_PlotLines] = hexToImVec4("#e4e4e6");
        colors[ImGuiCol_PlotLinesHovered] = hexToImVec4("#E69490");
        colors[ImGuiCol_PlotHistogram] = hexToImVec4("#C0B1E6");
        colors[ImGuiCol_PlotHistogramHovered] = hexToImVec4("#CBC0E6");
        colors[ImGuiCol_TableHeaderBg] = hexToImVec4("#403266");
        colors[ImGuiCol_TableBorderStrong] = hexToImVec4("#aaa7b3");
        colors[ImGuiCol_TableBorderLight] = hexToImVec4("#aaa7b3");
        colors[ImGuiCol_TextSelectedBg] = hexToImVec4("#C0B1E6", 0.35f);
        colors[ImGuiCol_DragDropTarget] = hexToImVec4("#C0B1E6", 0.9f);
        colors[ImGuiCol_NavHighlight] = hexToImVec4("#C0B1E6");

        style->WindowBorderSize = 0.0f;
      })
      .draw([] {
        if (!settingsOpen)
          return;
        static auto io = ImGui::GetIO();
        ImGui::SetNextWindowSize(
            ImVec2(io.DisplaySize.x / 5, io.DisplaySize.y / 2));
        ImGui::Begin("Overlayer");

        if (ImGui::Button("Add Text")) {
          auto textObject = new TextObject();
          textObject->x = 200.0f;
          textObject->y = 200.0f;
          textObject->name = "Text Object";
          textObject->text = "Hello, World!";
          textObject->fontSize = 12.0f;
          textObject->fontName = "bigFont.fnt";
          textObject->color = ccColor4B(255, 255, 255, 255);
          textObject->init();
          objects.push_back(textObject);
        }

        for (auto &obj : objects) {
          ImGui::BeginGroup();
          ImGui::InputText(fmt::format("##Name_{}", obj->uuid).c_str(),
                           &obj->name);
          ImGui::SameLine();
          if (ImGui::Button(fmt::format("Edit##{}_EDIT", obj->uuid).c_str())) {
            obj->settingsOpen = !obj->settingsOpen;
          }
          ImGui::SameLine();
          if (ImGui::Button(
                  fmt::format("Delete##{}_DELETE", obj->uuid).c_str())) {
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
                ImVec2(io.DisplaySize.x / 5, io.DisplaySize.y / 2),
                ImGuiCond_Once);
            ImGui::SetNextWindowPos(
                ImVec2(io.DisplaySize.x / 2, io.DisplaySize.y / 2),
                ImGuiCond_Once, ImVec2(0.5f, 0.5f));
            ImGui::Begin(
                fmt::format("{} Settings##{}_SETTINGS", obj->name, obj->uuid)
                    .c_str(),
                &obj->settingsOpen);

            obj->drawSettings();

            ImGui::End();
          }
        }
      });
}