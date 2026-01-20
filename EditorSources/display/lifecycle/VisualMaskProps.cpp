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
    const auto root = nsVisualHolder::GetRoot(target);
    return root && root != target;
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
                }
            }
        }

        ImGui::EndPopup();
    }

    if (ImGui::Button("Apply Mask")) {
        ImGui::OpenPopup(MASK_POPUP_ID);
    }

    for (int i = 0; i < target->masks.size(); i++) {
        auto mask = target->masks[i];
        nsString title;
        title.Format("Remove##mask%d", i);
        if (ImGui::Button(title)) {
            nsUndoService::Shared()->Push(new nsUndoVectorRemove(target->masks, mask));
        }
        ImGui::SameLine();
        ImGui::Text("Child Id: %s", mask->id.c_str());
    }
}