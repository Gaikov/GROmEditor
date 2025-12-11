//
// Created by Roman on 11/18/2025.
//

#include "ScenePropsView.h"
#include "Engine/display/container/VisualContainer2d.h"
#include "Core/Var.h"
#include "display/lifecycle/VisualsLifecycle.h"
#include "imgui/imgui.h"
#include "props/LayoutRefPropsView.h"
#include "props/SpritePropsView.h"
#include "props/TextLabelPropsView.h"
#include "props/VisualPropsView.h"

nsScenePropsView::nsScenePropsView() {
    auto &scenePath = _model->user.currentScene;
    scenePath.AddHandler(nsPropChangedName::CHANGED, [&](const nsBaseEvent *e) {
        _scene = _model->project.scenes.Get(scenePath);
    });

    _model->user.selectedObject.AddHandler(nsPropChangedName::CHANGED, [&](const nsBaseEvent *e) {
        _preselect = _model->user.selectedObject;
    });
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
            if (nsVisualObject2d *selected = _model->user.selectedObject) {
                nsVisualsLifecycle::Shared()->DrawProps(selected);
            } else {
                ImGui::Text("No selection");
            }
            ImGui::EndChild();
        }
    }

    ImGui::End();
}

void nsScenePropsView::DrawNode(nsVisualObject2d *node, int index) {
    nsVisualObject2d *selected = _model->user.selectedObject;
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None | ImGuiTreeNodeFlags_DrawLinesFull;
    const auto container = dynamic_cast<nsVisualContainer2d *>(node);
    if (container) {
        flags |= ImGuiTreeNodeFlags_DefaultOpen;
    } else {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }
    if (node == selected) {
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
            _preselect = node;
        }

        if (container) {
            auto &list = container->GetChildren();
            for (int i = 0; i < list.size(); i++) {
                DrawNode(list[i], index * 10 + i);
            }
        }

        nsString contextId;
        contextId.Format("context_%s", name.AsChar());

        if (ImGui::BeginPopupContextItem(contextId)) {
            nsVisualsLifecycle::Shared()->DrawContextMenu(node);
            ImGui::EndPopup();
        }

        ImGui::TreePop();
    }
}

void nsScenePropsView::PostDraw() {
    nsBaseView::PostDraw();
    if (_preselect != _model->user.selectedObject) {
        Log::Debug("Selected node changed");
        nsUndoService::Shared()->Push(new nsUndoVarChange(_model->user.selectedObject, _preselect));
    }
}
