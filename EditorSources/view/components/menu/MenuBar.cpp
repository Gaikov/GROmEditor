//
// Created by Roman on 11/28/2025.
//

#include "MenuBar.h"

#include "imgui/imgui.h"
void nsMenuBar::Draw() {
    if (ImGui::BeginMainMenuBar()) {
        for (const auto &child: _children) {
            child->Draw();
        }
        ImGui::EndMainMenuBar();
    }
}
