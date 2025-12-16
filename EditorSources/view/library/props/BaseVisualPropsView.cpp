//
// Created by Roman on 11/19/2025.
//

#include "BaseVisualPropsView.h"
#include "imgui/imgui.h"
#include "nsLib/locator/ServiceLocator.h"

nsBaseVisualPropsView::nsBaseVisualPropsView(const char *label) : _header(label) {
    _model = Locate<nsAppModel>();
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

void nsBaseVisualPropsView::DrawMenuSeparator(const bool hasPrevItems) {
    if (hasPrevItems) {
        ImGui::Separator();
    }
}
