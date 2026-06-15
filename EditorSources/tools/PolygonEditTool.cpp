// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file PolygonEditTool.cpp
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#include "PolygonEditTool.h"
#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVectorSet.h"
#include "Engine/RenAux.h"
#include "Engine/display/graphics/VisualPolygon.h"
#include "Platform/Desktop/DesktopKeyCodes.h"
#include "nsLib/locator/ServiceLocator.h"
#include "tools/SelectionTool.h"

nsPolygonEditTool::nsPolygonEditTool(nsVisualPolygon *polygon)
    : _polygon(polygon) {
    _appModel = Locate<nsAppModel>();
}

int nsPolygonEditTool::PickPoint(float x, float y) {
    if (!_polygon) return -1;

    const nsVec2 mouse = {x, y};
    for (int i = 0; i < static_cast<int>(_polygon->points.size()); ++i) {
        const nsVec2 worldPos = _polygon->origin.ToGlobal(_polygon->points[i]);
        if ((worldPos - mouse).Length() < POINT_RADIUS) {
            return i;
        }
    }
    return -1;
}

bool nsPolygonEditTool::OnPointerDown(nsVisualObject2d *scene, float x, float y, int pointerId) {
    if (pointerId != 0) return false;

    _dragIndex = PickPoint(x, y);
    if (_dragIndex < 0) return false;

    _dragPointOriginal = _polygon->points[_dragIndex];
    _dragOffset = _polygon->origin.ToGlobal(_dragPointOriginal) - nsVec2{x, y};
    return true;
}

bool nsPolygonEditTool::OnPointerMove(nsVisualObject2d *scene, float x, float y, int pointerId) {
    if (_dragIndex < 0) return false;

    const nsVec2 worldMouse = {x, y};
    _polygon->points[_dragIndex] = _polygon->origin.ToLocal(worldMouse + _dragOffset);
    return true;
}

bool nsPolygonEditTool::OnPointerUp(nsVisualObject2d *scene, float x, float y, int pointerId) {
    if (_dragIndex < 0) return false;

    auto &point = _polygon->points[_dragIndex];
    const nsVec2 newValue = point;
    point = _dragPointOriginal;
    nsUndoService::Shared()->Push(new nsUndoVectorSet<nsVec2>(
        _polygon->points, _dragIndex, newValue));
    _dragIndex = -1;
    return true;
}

bool nsPolygonEditTool::OnKeyDown(int key, bool repeated, int mods) {
    if (key == NS_KEY_ESCAPE) {
        _appModel->tools.ActivateTool<SelectionTool>();
        return true;
    }
    return false;
}

void nsPolygonEditTool::DrawOverlay() {
    if (!_polygon) return;

    const nsColor pointColor = {1.0, 1.0, 0.3, 0.9};
    const nsColor activeColor = {1.0, 0.6, 0.2, 1.0};

    for (int i = 0; i < static_cast<int>(_polygon->points.size()); ++i) {
        const nsVec2 worldPos = _polygon->origin.ToGlobal(_polygon->points[i]);
        nsGizmos::DrawCircle(worldPos, POINT_RADIUS, i == _dragIndex ? activeColor : pointColor);
    }
}
