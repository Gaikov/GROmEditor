//
// Created by Roman on 11/19/2025.
//

#include "VisualPropsView.h"

#include "Core/undo/UndoBatch.h"
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

    if (!nsVisualHolder::isRoot(target)) {
        DrawMenuSeparator(hasPrevItems);
        if (ImGui::MenuItem("Delete")) {
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
