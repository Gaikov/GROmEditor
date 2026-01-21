//
// Created by Roman on 1/21/2026.
//

#include "SceneTreeView.h"

#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVarChange.h"
#include "display/lifecycle/VisualsLifecycle.h"
#include "Engine/display/container/VisualContainer2d.h"
#include "imgui/imgui.h"

nsSceneTreeView::nsSceneTreeView() {
    auto &user = _model->project.user;
    auto &scenePath = user.currentScene;
    scenePath.AddHandler(nsPropChangedName::CHANGED, [&](const nsBaseEvent *e) {
        _scene = _model->project.scenes.Get(scenePath);
    });

    user.selectedObject.AddHandler(nsPropChangedName::CHANGED, [&](const nsBaseEvent *e) {
        _preselect = user.selectedObject;
    });
}

void nsSceneTreeView::Draw() {
    const auto &user = _model->project.user;

    if (_scene) {
        if (ImGui::CollapsingHeader("Scene Tree", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("Layout:");
            ImGui::SameLine();
            ImGui::Text(user.currentScene->c_str());

            ImGui::BeginChild("Tree List", ImVec2(0, 0),
                              ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeY,
                              ImGuiWindowFlags_HorizontalScrollbar);
            DrawNode(_scene, 1);
            ImGui::EndChild();
        }
    }
}

void nsSceneTreeView::DrawNode(nsVisualObject2d *node, int index) {
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

void nsSceneTreeView::PostDraw() {
    nsBaseView::PostDraw();
    auto &user = _model->project.user;
    if (_preselect != user.selectedObject) {
        Log::Debug("Selected node changed");
        nsUndoService::Shared()->Push(new nsUndoVarChange(user.selectedObject, _preselect));
    }
}
