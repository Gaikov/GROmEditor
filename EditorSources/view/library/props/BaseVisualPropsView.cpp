//
// Created by Roman on 11/19/2025.
//

#include "BaseVisualPropsView.h"
#include "imgui/imgui.h"

nsBaseVisualPropsView::nsBaseVisualPropsView(const char *label) : _header(label) {
}

void nsBaseVisualPropsView::DrawPanel(nsVisualObject2d *target) {
    if (ImGui::CollapsingHeader(_header.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::BeginChild(_header.c_str(), ImVec2(0, 0),
                          ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Borders,
                          ImGuiWindowFlags_HorizontalScrollbar);
        Draw(target);
        ImGui::EndChild();
    }
}
