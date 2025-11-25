//
// Created by Roman on 11/18/2025.
//

#include "ScenePropsView.h"
#include "Engine/display/container/VisualContainer2d.h"
#include "Core/Var.h"
#include "imgui/imgui.h"
#include "props/SpritePropsView.h"
#include "props/VisualPropsView.h"

nsScenePropsView::nsScenePropsView() {
    auto &scenePath = _model->user.currentScene;
    scenePath.AddHandler(nsPropChangedName::CHANGED, [&](const nsBaseEvent *e) {
        _scene = _model->project.scenes.Get(scenePath);
        _selected = _scene;
    });

    _propsViews.push_back(std::make_shared<nsVisualPropsView>());
    _propsViews.push_back(std::make_shared<nsSpritePropsView>());
}

void nsScenePropsView::Draw() {
    ImGui::Begin("Scene Properties");

    ImGui::Text("Layout:");
    ImGui::SameLine();
    ImGui::Text(_model->user.currentScene->c_str());

    if (_scene) {
        if (ImGui::CollapsingHeader("Scene Tree", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::BeginChild("Tree List", ImVec2(0, 0),
                              ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeY,
                              ImGuiWindowFlags_HorizontalScrollbar);
            DrawNode(_scene, 0);
            ImGui::EndChild();
        }

        if (ImGui::CollapsingHeader("Properties", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::BeginChild("Props List", ImVec2(0, 0),
                              ImGuiChildFlags_AutoResizeY,
                              ImGuiWindowFlags_HorizontalScrollbar);
            if (_selected) {
                for (const auto &propsView: _propsViews) {
                    if (propsView->IsSupport(_selected)) {
                        propsView->DrawPanel(_selected);
                    }
                }
            } else {
                ImGui::Text("No selection");
            }
            ImGui::EndChild();
        }
    }

    ImGui::End();
}

void nsScenePropsView::DrawNode(nsVisualObject2d *node, int index) {
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None | ImGuiTreeNodeFlags_DrawLinesFull;
    const auto container = dynamic_cast<nsVisualContainer2d *>(node);
    if (container) {
        flags |= ImGuiTreeNodeFlags_DefaultOpen;
    } else {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }
    if (node == _selected) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }

    nsString name;
    if (node->id.empty()) {
        name.Format("noname##%d", index);
    } else {
        name.Format("%s##%d", node->id.c_str(), index);
    }

    if (ImGui::TreeNodeEx(name, flags)) {
        if (ImGui::IsItemClicked()) {
            _selected = node;
        }

        if (container) {
            auto &list = container->GetChildren();
            for (int i = 0; i < list.size(); i++) {
                DrawNode(list[i], i);
            }
        }
        ImGui::TreePop();
    }
}
