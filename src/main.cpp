#include <Geode/Geode.hpp>
#include <imgui-cocos.hpp>
#include <vector>

#include "objects/Object.hpp"
#include "objects/impl/TextObject.hpp"
#include "imgui/imgui_stdlib.h"

using namespace geode::prelude;



static std::vector<Object*> objects;
static bool settingsOpen = false;

$on_mod(Loaded) {

    listenForKeybindSettingPresses("toggle-menu", [](Keybind const& keybind, bool down, bool repeat, double timestamp) {
        if (down) settingsOpen = !settingsOpen;
    });
	
	ImGuiCocos::get().setup([] {
        auto io = ImGui::GetIO();
        io.IniFilename = nullptr;
        auto* font = ImGui::GetIO().Fonts->AddFontFromFileTTF((Mod::get()->getResourcesDir() / "PretendardVariable.ttf").string().c_str(), 18.0f);
        io.FontDefault = font;
        io.Fonts->Build();

        auto style = &ImGui::GetStyle();
        style->WindowRounding = 10.0f;
        style->WindowPadding = ImVec2(10.0f, 10.0f);
        style->FrameRounding = 5.0f;
        style->Alpha = 1.0f;
        style->Colors[ImGuiCol_TitleBg] = ImVec4(0, 0.4f, 0.9f, 1.0f);
        style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0, 0.5f, 1.0f, 1.0f);
        style->Colors[ImGuiCol_WindowBg] = ImVec4(0, 0, 0, 1.0f);
        style->Colors[ImGuiCol_Button] = ImVec4(0, 0.4f, 0.9f, 1.0f);
        style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0, 0.5f, 1.0f, 1.0f);
        style->Colors[ImGuiCol_ButtonActive] = ImVec4(0, 0.6f, 1.0f, 1.0f);
        style->Colors[ImGuiCol_FrameBg] = ImVec4(0, 0.4f, 0.9f, 1.0f);
        style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0, 0.5f, 1.0f, 1.0f);
        style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0, 0.6f, 1.0f, 1.0f);

        style->WindowBorderSize = 0.0f;

    }).draw([] {
        if (!settingsOpen) return;
		static auto io = ImGui::GetIO();
		ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x / 5, io.DisplaySize.y / 2));
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

        for (auto& obj : objects) {
            ImGui::BeginGroup();
            ImGui::InputText("##Name", &obj->name);
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

        for (auto& obj : objects) {
            if (obj->settingsOpen) {
                ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x / 5, io.DisplaySize.y / 2), ImGuiCond_Once);
                ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x / 2, io.DisplaySize.y / 2), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
                ImGui::Begin(fmt::format("{} Settings##{}_SETTINGS", obj->name, obj->uuid).c_str(), &obj->settingsOpen);
                
                obj->drawSettings();

                ImGui::End();
            }
        }
    });
}