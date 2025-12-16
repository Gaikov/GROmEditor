//
// Created by Roman on 11/18/2025.
//

#include "ScenePropsView.h"
#include "Engine/display/container/VisualContainer2d.h"
#include "Core/Var.h"
#include "display/lifecycle/VisualsLifecycle.h"
#include "imgui/imgui.h"
#include "props/LayoutRefPropsView.h"
#include "display/lifecycle/SpritePropsView.h"
#include "props/TextLabelPropsView.h"
#include "props/VisualPropsView.h"

nsScenePropsView::nsScenePropsView() {
    auto &user = _model->project.user;
    auto &scenePath = user.currentScene;
    scenePath.AddHandler(nsPropChangedName::CHANGED, [&](const nsBaseEvent *e) {
        _scene = _model->project.scenes.Get(scenePath);
    });

    user.selectedObject.AddHandler(nsPropChangedName::CHANGED, [&](const nsBaseEvent *e) {
        _preselect = user.selectedObject;
    });
}

void nsScenePropsView::Draw() {
    auto &user = _model->project.user;

    ImGui::Begin("Scene Properties");

    ImGui::Text("Layout:");
    ImGui::SameLine();
    ImGui::Text(user.currentScene->c_str());

    if (_scene) {
        if (ImGui::CollapsingHeader("Scene Tree", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::BeginChild("Tree List", ImVec2(0, 0),
                              ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeY,
                              ImGuiWindowFlags_HorizontalScrollbar);
            DrawNode(_scene, 1);
            ImGui::EndChild();
        }

        if (ImGui::CollapsingHeader("Properties", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::BeginChild("Props List", ImVec2(0, 0),
                              ImGuiChildFlags_AutoResizeY,
                              ImGuiWindowFlags_HorizontalScrollbar);
            if (nsVisualObject2d *selected = user.selectedObject) {
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
    auto &user = _model->project.user;
    const nsVisualObject2d *selected = user.selectedObject;
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None | ImGuiTreeNodeFlags_DrawLinesFull;
    const auto container = dynamic_cast<nsVisualContainer2d *>(node);
    if (container) {
        flags |= ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick;
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

    const bool open = ImGui::TreeNodeEx(name, flags);
    if (ImGui::IsItemClicked()) {
        _preselect = node;
    }

    nsString contextId;
    contextId.Format("context_%s", name.AsChar());

    if (ImGui::BeginPopupContextItem(contextId)) {
        nsVisualsLifecycle::Shared()->DrawContextMenu(node);
        ImGui::EndPopup();
    }

    if (open) {
        if (container) {
            auto &list = container->GetChildren();
            for (int i = 0; i < list.size(); i++) {
                DrawNode(list[i], index * 10 + i);
            }
        }

        ImGui::TreePop();
    }
}

void nsScenePropsView::PostDraw() {
    nsBaseView::PostDraw();
    auto &user = _model->project.user;
    if (_preselect != user.selectedObject) {
        Log::Debug("Selected node changed");
        nsUndoService::Shared()->Push(new nsUndoVarChange(user.selectedObject, _preselect));
    }
}
