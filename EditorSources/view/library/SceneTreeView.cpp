//
// Created by Roman on 1/21/2026.
//

#include "SceneTreeView.h"

#include "SceneDragDrop.h"
#include "Core/undo/UndoBatch.h"
#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVarChange.h"
#include "display/lifecycle/VisualsLifecycle.h"
#include "Engine/display/container/VisualContainer2d.h"
#include "Engine/display/undo/UndoInsertVisualChild.h"
#include "Engine/display/undo/UndoRemoveChild.h"
#include "gizmos/VisualHolder.h"
#include "imgui/imgui.h"
#include "view/alerts/AlertPopup.h"

nsSceneTreeView::nsSceneTreeView() {
    auto &user = _model->project.user;
    auto &scenePath = user.currentScene;
    scenePath.AddHandler(nsPropChangedName::CHANGED, [&](const nsBaseEvent *e) {
        _scene = _model->project.scenes.Get(scenePath);
        UpdateCurrentScenePath();
    });
    _model->settings.projectPath.AddHandler(nsPropChangedName::CHANGED, [&](const nsBaseEvent *e) {
        UpdateCurrentScenePath();
    });

    user.selectedObject.AddHandler(nsPropChangedName::CHANGED, [&](const nsBaseEvent *e) {
        _preselect = user.selectedObject;
    });
    UpdateCurrentScenePath();
}

void nsSceneTreeView::Draw() {
    const auto &user = _model->project.user;
    ImGui::Begin("Scene Tree");

    if (_scene) {
        ImGui::Text("%s", _currentSceneRelativePath.AsChar());

        ImGui::BeginChild("Tree List", ImVec2(0, 0),
                          ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeY,
                          ImGuiWindowFlags_HorizontalScrollbar);
        DrawNode(_scene, 1);
        ImGui::EndChild();
    }
    ImGui::End();
}

void nsSceneTreeView::UpdateCurrentScenePath() {
    const nsFilePath projectPath = _model->GetProjectPath();
    const nsFilePath scenePath = _model->project.user.currentScene->c_str();
    _currentSceneRelativePath = projectPath.GetRelativePath(scenePath);
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
        name.Format("%s##%d", node->GetType(), index);
    } else {
        name.Format("%s (%s)##%d", node->id.c_str(), node->GetType(), index);
    }

    const bool open = ImGui::TreeNodeEx(name, flags);
    if (ImGui::IsItemClicked()) {
        _preselect = node;
    }

    nsString contextId;
    contextId.Format("context_%s", name.AsChar());

    DrawDragDrop(node);

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

void nsSceneTreeView::DrawDragDrop(nsVisualObject2d *node) {
    if (!nsVisualHolder::IsRoot(node) && ImGui::BeginDragDropSource()) {
        ImGui::SetDragDropPayload(DND_TREE_VISUAL_TYPE, &node, sizeof(nsVisualObject2d *));
        ImGui::Text("Moving: ", node->id.c_str());
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload(DND_TREE_VISUAL_TYPE)) {
            if (auto *dropped = *static_cast<nsVisualObject2d * const*>(payload->Data)) {
                Log::Info("Dropped: %s", dropped->id.c_str());
                if (node != dropped) {
                    OnDragDrop(dropped, node, GetDropMode(node));
                } else {
                    nsAlertPopup::Warning("Can't drop on itself");
                }
            }
        }
        ImGui::EndDragDropTarget();
    }
}

nsSceneTreeView::DropMode nsSceneTreeView::GetDropMode(nsVisualObject2d *node) const {
    if (nsVisualHolder::IsRoot(node)) {
        return DropMode::INSIDE;
    }

    const auto rectMin = ImGui::GetItemRectMin();
    const auto rectMax = ImGui::GetItemRectMax();
    const auto mouse = ImGui::GetMousePos();
    const float height = rectMax.y - rectMin.y;
    if (height <= 0) {
        return DropMode::BEFORE;
    }

    const float localY = mouse.y - rectMin.y;
    if (dynamic_cast<nsVisualContainer2d *>(node)) {
        if (localY < height * 0.25f) {
            return DropMode::BEFORE;
        }
        if (localY > height * 0.75f) {
            return DropMode::AFTER;
        }
        return DropMode::INSIDE;
    }

    return localY < height * 0.5f ? DropMode::BEFORE : DropMode::AFTER;
}

bool nsSceneTreeView::GetDropTarget(nsVisualObject2d *target,
                                    const DropMode mode,
                                    nsVisualContainer2d *&parent,
                                    int &index) const {
    if (mode == DropMode::INSIDE) {
        parent = dynamic_cast<nsVisualContainer2d *>(target);
        if (!parent) {
            return false;
        }
        index = static_cast<int>(parent->GetChildren().size());
        return true;
    }

    if (nsVisualHolder::IsRoot(target)) {
        return false;
    }

    parent = target->GetParent();
    if (!parent) {
        return false;
    }

    index = parent->GetChildIndex(target);
    if (mode == DropMode::AFTER) {
        index++;
    }
    return true;
}

void nsSceneTreeView::OnDragDrop(nsVisualObject2d *source, nsVisualObject2d *target, const DropMode mode) {
    nsVisualContainer2d *targetParent = nullptr;
    int targetIndex = 0;
    if (!GetDropTarget(target, mode, targetParent, targetIndex)) {
        nsAlertPopup::Warning("Can't drop on target");
        return;
    }

    const auto sourceParent = source->GetParent();
    if (!sourceParent) {
        const auto batch = new nsUndoBatch();
        batch->Add(new nsUndoInsertVisualChild(targetParent, source, targetIndex));
        batch->Add(new nsUndoVarChange(_model->project.user.selectedObject, source));
        _preselect = source;
        nsUndoService::Shared()->Push(batch);
        return;
    }

    const auto sourceIndex = sourceParent->GetChildIndex(source);
    if (targetParent == sourceParent) {
        if (sourceIndex < targetIndex) {
            targetIndex--;
        }
    }

    bool containsTarget = false;
    if (source == targetParent) {
        containsTarget = true;
    } else if (const auto sourceContainer = dynamic_cast<nsVisualContainer2d *>(source)) {
        sourceContainer->IterateRecursive([&](const nsVisualObject2d *child) {
            if (child == targetParent) {
                containsTarget = true;
            }
            return true;
        });
    }
    if (containsTarget) {
        nsAlertPopup::Warning("Can't drop on child");
        return;
    }

    const auto targetAngle = targetParent->origin.ToLocalAngle(source->origin.ToGlobalAngle(0));
    const auto targetPos = targetParent->origin.ToLocal(source->origin.ToGlobal(nsVec2(0, 0)));

    const auto batch = new nsUndoBatch();
    batch->Add(new nsUndoRemoveChild(source));
    batch->Add(new nsUndoInsertVisualChild(targetParent, source, targetIndex));
    batch->Add(new nsUndoVarChange(source->origin.pos, targetPos));
    batch->Add(new nsUndoVarChange(source->origin.angle, targetAngle));
    nsUndoService::Shared()->Push(batch);
}
