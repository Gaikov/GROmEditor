//
// Created by Roman on 11/13/2025.
//

#include "AlertPopup.h"
#include "imgui/imgui.h"
#include "view/ViewsRoot.h"

static int itCounter = 0;

nsAlertPopup::nsAlertPopup(const char *title, const char *message) {
    _title = title;
    _message = message;
    _id = _title + std::to_string(itCounter++);
}

void nsAlertPopup::Draw() {
    if (_show) {
        _show = false;
        ImGui::OpenPopup(_id.c_str());
    }

    //if (ImGui::BeginPopup(_id.c_str(), ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)) {
    if (ImGui::BeginPopupModal(_id.c_str(), &visible, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)) {
        ImGui::Text("Test popup content");

        if (ImGui::Button("Open")) {
            nsViewsRoot::Shared()->AddView<nsAlertPopup>("Info", "Another popup!");
        }

        ImGui::SameLine();
        if (ImGui::Button("Close")) {
            ImGui::CloseCurrentPopup();
            visible = false;
        }
        ImGui::EndPopup();
    }

    if (!visible) {
        nsViewsRoot::Shared()->RemoveView(this);
    }

    /*const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopup("test_popup1", ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text(_message.c_str());
        ImGui::Separator();

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }


    if (_show) {
        _show = false;
        ImGui::OpenPopup("test_popup1");
    }*/
}
