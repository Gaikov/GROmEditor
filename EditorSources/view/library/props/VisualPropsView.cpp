//
// Created by Roman on 11/19/2025.
//

#include "VisualPropsView.h"

#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVarChange.h"
#include "imgui/imgui.h"

void nsVisualPropsView::Draw(nsVisualObject2d *target) {
    _id = target->id.c_str();
    if (ImGui::InputText("ID", _id.AsChar(), nsString::MAX_SIZE - 1)) {
        const std::string id = _id.AsChar();
        nsUndoService::Shared()->Push(new nsUndoVarChange(target->id, id));
    }
}
