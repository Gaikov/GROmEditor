// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file PolygonEditTool.cpp
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#include "PolygonEditTool.h"
#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVectorAdd.h"
#include "Core/undo/UndoVectorInsert.h"
#include "Core/undo/UndoVectorRemoveAt.h"
#include "Core/undo/UndoVectorSet.h"
#include "Engine/KeyCodes.h"
#include "Engine/RenAux.h"
#include "Engine/RenManager.h"
#include "Engine/display/graphics/VisualPolygon.h"
#include "nsLib/Line2.h"
#include "nsLib/locator/ServiceLocator.h"
#include "tools/SelectionTool.h"
#include <limits>

nsPolygonEditTool::nsPolygonEditTool(nsVisualPolygon *polygon)
    : _polygon(polygon) {
    _appModel = Locate<nsAppModel>();
    auto *dev = nsRenDevice::Shared()->Device();
    _handleTex = dev->TextureLoad("default/editor/drag-point.png", false, TF_RGBA, TLF_PREMULTIPLY_ALPHA);
    _handleRenState = dev->StateLoad("default/rs/sprite_pma.ggrs");
    _desc.tex = _handleTex;
    _desc.premultiplyAlpha = true;
    _desc.ResetSize();
    _desc.ComputeCenter();
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

int nsPolygonEditTool::FindNearestEdge(const nsVec2 &worldPos) const {
    if (!_polygon || _polygon->points.size() < 2) {
        return -1;
    }

    int nearestEdge = -1;
    float nearestDistance = std::numeric_limits<float>::max();
    const int pointsCount = static_cast<int>(_polygon->points.size());
    for (int i = 0; i < pointsCount; ++i) {
        const nsVec2 start = _polygon->origin.ToGlobal(_polygon->points[i]);
        const nsVec2 end = _polygon->origin.ToGlobal(_polygon->points[(i + 1) % pointsCount]);
        nsLine2 line(start, end);
        nsVec2 closest;
        line.ClosestPointOnSegment(worldPos, closest);
        const nsVec2 delta = worldPos - closest;
        const float distance = delta.x * delta.x + delta.y * delta.y;
        if (distance < nearestDistance) {
            nearestDistance = distance;
            nearestEdge = i;
        }
    }
    return nearestEdge;
}

void nsPolygonEditTool::UpdateMousePosition(const float x, const float y) {
    _mouseWorld = {x, y};
}

bool nsPolygonEditTool::OnPointerDown(nsVisualObject2d *scene, float x, float y, int pointerId) {
    UpdateMousePosition(x, y);

    if (_altPressed) {
        if (pointerId != NS_POINTER_PRIMARY) {
            return false;
        }

        const int edgeIndex = FindNearestEdge(_mouseWorld);
        if (edgeIndex < 0) {
            return false;
        }

        const nsVec2 point = _polygon->origin.ToLocal(_mouseWorld);
        const int insertIndex = edgeIndex + 1;
        if (insertIndex == static_cast<int>(_polygon->points.size())) {
            nsUndoService::Shared()->Push(new nsUndoVectorAdd<nsVec2>(_polygon->points, point));
        } else {
            nsUndoService::Shared()->Push(new nsUndoVectorInsert<nsVec2>(
                _polygon->points, insertIndex, point));
        }
        _hoverIndex = -1;
        return true;
    }

    if (pointerId == NS_POINTER_SECONDARY) {
        if (_dragIndex >= 0) {
            return true;
        }

        const int pointIndex = PickPoint(x, y);
        if (pointIndex < 0) {
            return false;
        }

        if (_polygon->points.size() > 3) {
            nsUndoService::Shared()->Push(new nsUndoVectorRemoveAt<nsVec2>(
                _polygon->points, pointIndex));
            _hoverIndex = PickPoint(x, y);
        }
        return true;
    }

    if (pointerId != NS_POINTER_PRIMARY) return false;

    _dragIndex = PickPoint(x, y);
    if (_dragIndex < 0) return false;

    _dragPointOriginal = _polygon->points[_dragIndex];
    _dragOffset = _polygon->origin.ToGlobal(_dragPointOriginal) - nsVec2{x, y};
    return true;
}

bool nsPolygonEditTool::OnPointerMove(nsVisualObject2d *scene, float x, float y, int pointerId) {
    UpdateMousePosition(x, y);

    if (_dragIndex < 0) {
        _hoverIndex = _altPressed ? -1 : PickPoint(x, y);
        return false;
    }

    if (pointerId != NS_POINTER_PRIMARY) {
        return false;
    }

    _polygon->points[_dragIndex] = _polygon->origin.ToLocal(_mouseWorld + _dragOffset);
    return true;
}

bool nsPolygonEditTool::OnPointerUp(nsVisualObject2d *scene, float x, float y, int pointerId) {
    UpdateMousePosition(x, y);
    if (pointerId != NS_POINTER_PRIMARY || _dragIndex < 0) return false;

    auto &point = _polygon->points[_dragIndex];
    const nsVec2 newValue = point;
    point = _dragPointOriginal;
    nsUndoService::Shared()->Push(new nsUndoVectorSet<nsVec2>(
        _polygon->points, _dragIndex, newValue));
    _dragIndex = -1;
    _hoverIndex = _altPressed ? -1 : PickPoint(x, y);
    return true;
}

bool nsPolygonEditTool::OnKeyDown(int key, bool repeated, int mods) {
    if (key == NS_KEY_LEFT_ALT || key == NS_KEY_RIGHT_ALT) {
        _altPressed = true;
        _hoverIndex = -1;
        return true;
    }

    if (key == NS_KEY_ESCAPE) {
        _appModel->tools.ActivateTool<SelectionTool>();
        return true;
    }
    return false;
}

bool nsPolygonEditTool::OnKeyUp(const int key, int mods) {
    if (key == NS_KEY_LEFT_ALT || key == NS_KEY_RIGHT_ALT) {
        _altPressed = false;
        _hoverIndex = PickPoint(_mouseWorld.x, _mouseWorld.y);
        return true;
    }
    return false;
}

void nsPolygonEditTool::DrawOverlay() {
    if (!_polygon || !_handleTex) return;

    auto *dev = nsRenDevice::Shared()->Device();

    if (_altPressed && _dragIndex < 0) {
        const int edgeIndex = FindNearestEdge(_mouseWorld);
        if (edgeIndex >= 0) {
            const int pointsCount = static_cast<int>(_polygon->points.size());
            const nsVec2 p1 = _polygon->origin.ToGlobal(_polygon->points[edgeIndex]);
            const nsVec2 p2 = _polygon->origin.ToGlobal(_polygon->points[(edgeIndex + 1) % pointsCount]);
            const nsColor color = _appModel->settings.selectionColor;
            nsGizmos::DrawDashedLine(p1, _mouseWorld, color, nsColor::black, DASH_LENGTH);
            nsGizmos::DrawDashedLine(p2, _mouseWorld, color, nsColor::black, DASH_LENGTH);
        }
    }

    dev->StateApply(_handleRenState);

    for (int i = 0; i < static_cast<int>(_polygon->points.size()); ++i) {
        _desc.pos = _polygon->origin.ToGlobal(_polygon->points[i]);
        _desc.color = i == _dragIndex || i == _hoverIndex ? nsColor::green : nsColor::white;
        _desc.Draw(dev);
    }
}
