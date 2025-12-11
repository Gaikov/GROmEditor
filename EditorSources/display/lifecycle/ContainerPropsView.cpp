//
// Created by Roman on 12/11/2025.
//

#include "ContainerPropsView.h"

#include "Core/undo/UndoBatch.h"
#include "Engine/display/container/VisualContainer2d.h"
#include "Engine/display/text/TextLabel.h"
#include "Engine/display/undo/UndoAddChild.h"

bool nsContainerPropsView::IsSupport(nsVisualObject2d *target) {
    return dynamic_cast<nsVisualContainer2d*>(target);
}

bool nsContainerPropsView::DrawContextMenu(nsVisualObject2d *target, bool hasPrevItems) {
    const auto c = dynamic_cast<nsVisualContainer2d*>(target);

    if (ImGui::BeginMenu("Create...")) {
        if (ImGui::MenuItem("Label")) {
            nsVisualObject2d *label = new nsTextLabel();
            label->id = "Label";
            const auto batch = new nsUndoBatch();
            batch->Add(new nsUndoAddChild(c, label));
            batch->Add(new nsUndoVarChange(_model->user.selectedObject, label));
            nsUndoService::Shared()->Push(batch);
        }

        ImGui::EndMenu();
    }
    return true;
}

void nsContainerPropsView::Draw(nsVisualObject2d *target) {
    const auto c = dynamic_cast<nsVisualContainer2d*>(target);
    _interactiveInput.Draw(c->interactiveChildren);
}
