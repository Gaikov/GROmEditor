//
// Created by Roman on 11/19/2025.
//

#include "VisualPropsView.h"

#include "Core/undo/UndoBatch.h"
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
