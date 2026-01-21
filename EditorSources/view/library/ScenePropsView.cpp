//
// Created by Roman on 11/18/2025.
//

#include "ScenePropsView.h"
#include "Engine/display/container/VisualContainer2d.h"
#include "Core/Var.h"
#include "display/lifecycle/VisualsLifecycle.h"
#include "imgui/imgui.h"
#include "props/LayoutRefPropsView.h"
#include "props/TextLabelPropsView.h"

nsScenePropsView::nsScenePropsView() {
    auto &user = _model->project.user;
    auto &scenePath = user.currentScene;
    scenePath.AddHandler(nsPropChangedName::CHANGED, [&](const nsBaseEvent *e) {
        _scene = _model->project.scenes.Get(scenePath);
    });
}

void nsScenePropsView::Draw() {
    auto &user = _model->project.user;

    ImGui::Begin("Inspector");

    if (_scene) {
        ImGui::BeginChild("Props List", ImVec2(0, 0),
                          ImGuiChildFlags_AutoResizeY,
                          ImGuiWindowFlags_HorizontalScrollbar);
        if (nsVisualObject2d *selected = user.selectedObject) {
            nsVisualsLifecycle::Shared()->DrawProps(selected);
        } else {
            ImGui::Text("No selection");
        }
        ImGui::EndChild();
    } else {
        ImGui::Text("No selected object");
    }

    ImGui::End();
}
