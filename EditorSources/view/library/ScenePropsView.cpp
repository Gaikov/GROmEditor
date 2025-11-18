//
// Created by Roman on 11/18/2025.
//

#include "ScenePropsView.h"

#include "Core/Var.h"
#include "imgui/imgui.h"

extern nsVar    *sv_last_layout;

void nsScenePropsView::Draw() {

    ImGui::Begin("Scene Properties");

    ImGui::Text("Layout:");
    ImGui::SameLine();
    ImGui::Text(sv_last_layout->String());

    if (ImGui::Button("Test")) {

    }

    ImGui::End();
}
