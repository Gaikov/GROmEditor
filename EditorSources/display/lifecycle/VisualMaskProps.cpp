//
// Created by Roman on 1/19/2026.
//

#include "VisualMaskProps.h"

#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVectorAdd.h"
#include "Core/undo/UndoVectorRemove.h"
#include "gizmos/VisualHolder.h"
#include "imgui/imgui.h"

#define MASK_POPUP_ID "Mask Props"

bool nsVisualMaskProps::IsSupport(nsVisualObject2d *target) {
    return nsVisualHolder::GetRoot(target);
}

void nsVisualMaskProps::Draw(nsVisualObject2d *target) {
    const auto root = nsVisualHolder::GetRoot(target);

    _masks.clear();
    root->GetChildrenRecursive(_masks);

    if (ImGui::BeginPopup(MASK_POPUP_ID)) {

        for (auto mask : _masks) {
            if (mask != target) {
                const bool hasMask = target->HasMask(mask);
                if (ImGui::MenuItem(mask->id.c_str(), nullptr, hasMask)) {
                    if (hasMask) {
                        nsUndoService::Shared()->Push(new nsUndoVectorRemove(target->masks, mask));
                    } else {
                        nsUndoService::Shared()->Push(new nsUndoVectorAdd(target->masks, mask));
                    }
                    //nsUndoService::Shared()->Push(new nsUndoVarChange(target, mask));
                }
            }
        }

        ImGui::EndPopup();
    }

    if (ImGui::Button("Add Mask")) {
        ImGui::OpenPopup(MASK_POPUP_ID);
    }
}