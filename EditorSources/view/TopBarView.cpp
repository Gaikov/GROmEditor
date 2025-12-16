//
// Created by Roman on 11/24/2025.
//

#include "TopBarView.h"

#include "imgui/imgui.h"

void nsTopBarView::Draw() {
    if (ImGui::Begin("Top Bar", 0,
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize)) {
        auto &user = _model->project.user;
        nsColor c = user.backColor.GetValue();

        if (ImGui::ColorEdit4("Back", c, ImGuiColorEditFlags_NoInputs)) {
            user.backColor.SetValue(c);
        }

        ImGui::SameLine();
        float zoom = user.zoom * 100.0f;
        if (ImGui::InputFloat("Zoom", &zoom, 10, 50, "%.2f")) {
            user.zoom = zoom / 100.0f;
        }

        ImGui::SameLine();
        if (ImGui::Button("100%")) {
            user.zoom = 1.0f;
        }
    }
    ImGui::End();
}
