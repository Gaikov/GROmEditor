//
// Created by Roman on 11/19/2025.
//

#include "VisualPropsView.h"

#include "Core/undo/UndoBatch.h"
#include "Core/undo/UndoVarChange.h"
#include "Engine/display/VisualType.h"
#include "Engine/display/container/VisualContainer2d.h"
#include "Engine/display/undo/UndoAddChild.h"
#include "Engine/display/undo/UndoInsertVisualChild.h"
#include "Engine/display/undo/UndoRemoveChild.h"
#include "gizmos/VisualHolder.h"

void nsVisualPropsView::Draw(nsVisualObject2d *target) {
    _idInput.Draw(target->id);
    _bindingInput.Draw(target->bindingId);
    _visibleInput.Draw(target->visible);
    _posInput.Draw(target->origin.pos);
    _scaleInput.Draw(target->origin.scale);
    _angleInput.Draw(target->origin.angle);
}

bool nsVisualPropsView::DrawContextMenu(nsVisualObject2d *target, const bool hasPrevItems) {

    if (!nsVisualHolder::IsRoot(target)) {
        DrawMenuSeparator(hasPrevItems);

        const auto parent = target->GetParent();
        if (!parent) {
            return false;
        }

        const auto &children = parent->GetChildren();
        if (children[0] != target) {
            if (ImGui::MenuItem("Move Up")) {
                const auto batch = new nsUndoBatch();
                batch->Add(new nsUndoRemoveChild(target));
                batch->Add(new nsUndoInsertVisualChild(parent, target, parent->GetChildIndex(target) - 1));
                nsUndoService::Shared()->Push(batch);
            }
        }

        if (children[children.size() - 1] != target) {
            if (ImGui::MenuItem("Move Down")) {
                const auto batch = new nsUndoBatch();
                batch->Add(new nsUndoRemoveChild(target));
                batch->Add(new nsUndoInsertVisualChild(parent, target, parent->GetChildIndex(target) + 1));
                nsUndoService::Shared()->Push(batch);
            }
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Wrap to container")) {
            if (auto *container = _model->project.scenes.Create<nsVisualContainer2d>(nsVisualType::CONTAINER)) {
                const int targetIndex = parent->GetChildIndex(target);

                container->id = target->id.empty() ? "container" : target->id + "_container";
                container->origin.pos = target->origin.pos.GetValue();
                container->origin.scale = target->origin.scale.GetValue();
                container->origin.angle = target->origin.angle.GetValue();

                const auto batch = new nsUndoBatch();
                batch->Add(new nsUndoRemoveChild(target));
                batch->Add(new nsUndoInsertVisualChild(parent, container, targetIndex));
                batch->Add(new nsUndoInsertVisualChild(container, target, 0));
                batch->Add(new nsUndoVarChange(target->origin.pos, nsVec2(0, 0)));
                batch->Add(new nsUndoVarChange(target->origin.scale, nsVec2(1, 1)));
                batch->Add(new nsUndoVarChange(target->origin.angle, 0.0f));
                batch->Add(new nsUndoVarChange(_model->project.user.selectedObject,
                                               static_cast<nsVisualObject2d *>(container)));
                nsUndoService::Shared()->Push(batch);
            }
        }

        if (ImGui::MenuItem("Duplicate")) {
            if (const auto clone = _model->project.scenes.Clone(target)) {
                const auto batch = new nsUndoBatch();
                clone->id = target->id + "_copy";
                batch->Add(new nsUndoAddChild(target->GetParent(), clone));
                batch->Add(new nsUndoVarChange(_model->project.user.selectedObject, clone));
                nsUndoService::Shared()->Push(batch);
            }
        }

        if (ImGui::MenuItem("Remove")) {
            auto &user = _model->project.user;
            const auto batch = new nsUndoBatch();

            batch->Add(new nsUndoRemoveChild(target));
            if (user.selectedObject == target) {
                batch->Add(new nsUndoVarChange(user.selectedObject, static_cast<nsVisualObject2d *>(nullptr)));
            }

            nsUndoService::Shared()->Push(batch);
        }
        return true;
    }
    return false;
}
