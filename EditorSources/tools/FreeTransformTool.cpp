// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file FreeTransformTool.cpp
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#include "FreeTransformTool.h"
#include "Core/undo/UndoBatch.h"
#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVarChange.h"
#include "Engine/RenAux.h"
#include "Engine/RenManager.h"
#include "Platform/Desktop/DesktopKeyCodes.h"
#include "nsLib/locator/ServiceLocator.h"
#include "scene/SceneUtils.h"
#include "tools/SelectionTool.h"

FreeTransformTool::FreeTransformTool(nsVisualObject2d *target)
    : _target(target) {
    _appModel = Locate<nsAppModel>();
    auto *dev = nsRenDevice::Shared()->Device();
    _handleTex = dev->TextureLoad("default/editor/rotate-point.png", false, TF_RGBA, TLF_PREMULTIPLY_ALPHA);
    _handleRenState = dev->StateLoad("default/rs/sprite_pma.ggrs");
    _desc.tex = _handleTex;
    _desc.premultiplyAlpha = true;
    _desc.ResetSize();
    _desc.ComputeCenter();
}

nsVec2 FreeTransformTool::GetCornerWorld(int index) const {
    nsRect rect;
    _target->GetLocalBounds(rect);

    nsVec2 local;
    switch (index) {
        case 0: local = {rect.minX(), rect.minY()}; break;
        case 1: local = {rect.maxX(), rect.minY()}; break;
        case 2: local = {rect.maxX(), rect.maxY()}; break;
        default: local = {rect.minX(), rect.maxY()}; break;
    }
    return _target->origin.ToGlobal(local);
}

int FreeTransformTool::FindCorner(float x, float y) {
    if (!_target) return -1;

    const nsVec2 mouse = {x, y};
    for (int i = 0; i < 4; ++i) {
        const nsVec2 corner = GetCornerWorld(i);
        if ((corner - mouse).Length() < HANDLE_RADIUS) {
            return i;
        }
    }
    return -1;
}

bool FreeTransformTool::OnPointerDown(nsVisualObject2d *scene, float x, float y, int pointerId) {
    if (pointerId != 0) return false;
    if (!_target) return false;

    _dragHandle = FindCorner(x, y);
    if (_dragHandle >= 0) {
        _mode = ROTATE;
        _startPos = _target->origin.pos;
        _startAngle = _target->origin.angle;
        const nsVec2 worldCenter = _target->origin.ToGlobal(nsVec2{0, 0});
        _startMouseAngle = nsVec2(x - worldCenter.x, y - worldCenter.y).GetAngle();
        return true;
    }

    if (_target->HitTest(x, y)) {
        _mode = MOVE;
        _startPos = _target->origin.pos;
        auto *parent = _target->origin.GetParent();
        const nsVec2 mouseLocal = parent ? parent->ToLocal(nsVec2{x, y}) : nsVec2{x, y};
        _mouseOffset = _startPos - mouseLocal;
        return true;
    }

    return false;
}

bool FreeTransformTool::OnPointerMove(nsVisualObject2d *scene, float x, float y, int pointerId) {
    if (!_target) return false;

    if (_mode == MOVE) {
        auto *parent = _target->origin.GetParent();
        const nsVec2 mouseLocal = parent ? parent->ToLocal(nsVec2{x, y}) : nsVec2{x, y};
        _target->origin.pos = mouseLocal + _mouseOffset;
        return true;
    }

    if (_mode == ROTATE) {
        const nsVec2 worldCenter = _target->origin.ToGlobal(nsVec2{0, 0});
        const float angle = nsVec2(x - worldCenter.x, y - worldCenter.y).GetAngle();
        const float delta = angle - _startMouseAngle;
        _target->origin.angle = _startAngle + delta;
        return true;
    }

    _hoverHandle = FindCorner(x, y);
    return false;
}

bool FreeTransformTool::OnPointerUp(nsVisualObject2d *scene, float x, float y, int pointerId) {
    const auto prevMode = _mode;

    if (_mode == MOVE) {
        const nsVec2 newPos = _target->origin.pos;
        _target->origin.pos = _startPos;
        nsUndoService::Shared()->Push(new nsUndoVarChange(_target->origin.pos, newPos));
    } else if (_mode == ROTATE) {
        const float newAngle = _target->origin.angle;
        _target->origin.angle = _startAngle;
        nsUndoService::Shared()->Push(new nsUndoVarChange(_target->origin.angle, newAngle));
    }

    _mode = NONE;
    _dragHandle = -1;

    if (_target) {
        _hoverHandle = FindCorner(x, y);
    }

    return prevMode != NONE;
}

bool FreeTransformTool::OnKeyDown(int key, bool repeated, int mods) {
    if (key == NS_KEY_ESCAPE) {
        _appModel->tools.ActivateTool<SelectionTool>();
        return true;
    }
    return false;
}

void FreeTransformTool::DrawOverlay() {
    if (!_target || !_handleTex) return;

    auto *dev = nsRenDevice::Shared()->Device();
    dev->StateApply(_handleRenState);

    nsSceneUtils::DrawBounds(_target, _appModel->settings.selectionColor);

    const float objAngle = _target->origin.angle;
    const nsVec2 savedPos = _desc.pos;

    const float cornerOffsets[4] = {
        nsMath::ToRad(270.0f - 45.0f),  // bottom-left
        nsMath::ToRad(180.0f - 45.0f),  // bottom-right
        nsMath::ToRad(90.0f - 45.0f),   // top-right
        nsMath::ToRad(0.0f - 45.0f),    // top-left
    };

    for (int i = 0; i < 4; ++i) {
        _transform.pos = GetCornerWorld(i);
        _transform.angle = objAngle + cornerOffsets[i];

        nsMatrix m;
        _transform.GetLocal().ToMatrix3(m);
        dev->LoadMatrix(m);

        _desc.pos = {0, 0};
        const bool active = i == _dragHandle || i == _hoverHandle;
        _desc.color = active ? nsColor::green : nsColor::white;
        _desc.Draw(dev);
    }

    _desc.pos = savedPos;

    _transform.Reset();
    nsMatrix identity;
    _transform.GetLocal().ToMatrix3(identity);
    dev->LoadMatrix(identity);
}
