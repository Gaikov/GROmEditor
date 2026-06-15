//
// Created by Roman on 6/12/2026.
//

#include "PolygonPropsView.h"
#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVectorAdd.h"
#include "Core/undo/UndoVectorRemoveAt.h"
#include "Engine/display/graphics/VisualPolygon.h"
#include "imgui/imgui.h"
#include "tools/PolygonEditTool.h"
#include "view/components/Vec2InputUndo.h"

bool nsPolygonPropsView::IsSupport(nsVisualObject2d *target) {
    return dynamic_cast<nsVisualPolygon*>(target) != nullptr;
}

void nsPolygonPropsView::Draw(nsVisualObject2d *target) {
    const auto poly = dynamic_cast<nsVisualPolygon*>(target);
    const auto undo = nsUndoService::Shared();

    _colorInput.Draw(poly->color);

    ImGui::Spacing();
    ImGui::Text("Points: %zu", poly->points.size());
    ImGui::SameLine();
    if (ImGui::Button("Add Point")) {
        undo->Push(new nsUndoVectorAdd<nsVec2>(poly->points, {0, 0}));
    }
    if (poly->points.size() > 3) {
        ImGui::SameLine();
        if (ImGui::Button("Remove Last")) {
            undo->Push(new nsUndoVectorRemoveAt<nsVec2>(poly->points,
                static_cast<int>(poly->points.size()) - 1));
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Edit")) {
        _model->tools.ActivateTool<nsPolygonEditTool>(poly);
    }

    for (size_t i = 0; i < poly->points.size(); ++i) {
        nsVec2InputUndo<nsVec2>::DrawField(StrPrintf("Point %zu", i), poly->points, static_cast<int>(i));
    }
}
