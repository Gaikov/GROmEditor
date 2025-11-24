//
// Created by Roman on 11/24/2025.
//

#include "TopBarView.h"

#include "imgui/imgui.h"

void nsTopBarView::Draw() {
    if (ImGui::Begin("Top Bar", 0,
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize)) {
        nsColor c = _model->user.backColor.GetValue();
        if (ImGui::ColorEdit4("Back", c, ImGuiColorEditFlags_NoInputs)) {
            _model->user.backColor.SetValue(c);
        }

        ImGui::SameLine();

        //ImGui::InputFloat("Zoom", &_model->user.zoom);
    }
    ImGui::End();
}
