// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file SelectionTool.cpp
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#include "SelectionTool.h"
#include "Engine/display/container/VisualContainer2d.h"
#include "Core/undo/UndoVarChange.h"
#include "Core/undo/UndoService.h"
#include "nsLib/locator/ServiceLocator.h"
#include "scene/SceneUtils.h"

SelectionTool::SelectionTool() {
    _appModel = Locate<nsAppModel>();
}

bool SelectionTool::OnPointerDown(nsVisualObject2d *scene, float x, float y, int pointerId) {
    if (pointerId != 0) return false;

    _hoverTarget = nullptr;

    auto target = PickObject(scene, x, y);
    auto &selected = _appModel->project.user.selectedObject;
    if (selected.GetValue() == target) return false;

    nsUndoService::Shared()->Push(new nsUndoVarChange(selected, target));
    return true;
}

bool SelectionTool::OnPointerMove(nsVisualObject2d *scene, float x, float y, int pointerId) {
    _hoverTarget = PickObject(scene, x, y);
    return false;
}

bool SelectionTool::OnPointerUp(nsVisualObject2d *scene, float x, float y, int pointerId) {
    return false;
}

void SelectionTool::DrawOverlay() {
    nsSceneUtils::DrawBounds(_hoverTarget, _appModel->settings.hoverColor);
}

nsVisualObject2d *SelectionTool::PickObject(nsVisualObject2d *node, float x, float y) {
    if (!node || !node->visible) return nullptr;

    if (auto container = dynamic_cast<nsVisualContainer2d *>(node)) {
        const auto &children = container->GetChildren();
        for (auto it = children.rbegin(); it != children.rend(); ++it) {
            if (auto hit = PickObject(*it, x, y))
                return hit;
        }
        return node->nsVisualObject2d::HitTest(x, y) ? node : nullptr;
    }

    return node->HitTest(x, y) ? node : nullptr;
}
