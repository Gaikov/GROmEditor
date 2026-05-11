//
// Created by Roman on 10/21/2024.
//

#include "SceneView.h"

#include "Engine/RenAux.h"
#include "Engine/TimeFormat.h"
#include "Engine/utils/AppUtils.h"
#include "Engine/display/sprite/Sprite.h"
#include "events/EditorEventBus.h"
#include "nsLib/locator/ServiceLocator.h"
#include <algorithm>
#include <cmath>

nsSceneView::nsSceneView() {
    _appModel = Locate<nsAppModel>();
    nsEditorEventBus::Shared()->AddHandler(nsEditorEventName::FIT_SCENE_TO_VIEW, [&](const nsBaseEvent *) {
        FitSceneToView();
    });
    nsEditorEventBus::Shared()->AddHandler(nsEditorEventName::CENTER_SCENE_100, [&](const nsBaseEvent *) {
        CenterSceneAt100();
    });
}

void nsSceneView::Destroy() {
    if (_scene) {
        RemoveChild(_scene);
    }
    nsVisualContainer2d::Destroy();
}

void nsSceneView::SetScene(nsVisualObject2d *scene) {
    if (_scene) {
        RemoveChild(_scene);
    }
    _dragging = false;
    _wheelAnchorActive = false;
    _scene = scene;
    if (_scene) {
        AddChild(_scene);
    }
}

void nsSceneView::Loop() {
    if (_scene) {
        UpdateCamera();
    }

    nsVisualContainer2d::Loop();
}

void nsSceneView::UpdateCamera() {
    auto &user = _appModel->project.user;
    const nsVec2 targetScale = GetTargetScale(user.zoom);

    const nsVec2 scale = origin.scale;
    origin.scale = {
        nsMath::MoveExp(scale.x, targetScale.x, CAMERA_MOVE_SPEED, g_frameTime),
        nsMath::MoveExp(scale.y, targetScale.y, CAMERA_MOVE_SPEED, g_frameTime)
    };
    origin.angle = nsMath::MoveExp(origin.angle, _targetAngle, CAMERA_ANGLE_SPEED, g_frameTime);

    if (_wheelAnchorActive) {
        nsMatrix2 anchorMatrix;
        anchorMatrix.Transform(origin.pos, origin.scale, origin.angle);
        const nsVec2 anchorGlobalPoint = anchorMatrix.TransformPoint(_wheelAnchorLocalPoint);
        const nsVec2 deltaPos = _mousePos - anchorGlobalPoint;
        origin.pos = origin.pos + deltaPos;

        user.sceneX = origin.pos->x;
        user.sceneY = origin.pos->y;

        if (std::fabs(origin.scale->x - targetScale.x) < ANCHOR_SCALE_EPS
            && std::fabs(origin.scale->y - targetScale.y) < ANCHOR_SCALE_EPS) {
            _wheelAnchorActive = false;
        }
    } else {
        nsVec2 pos = origin.pos;
        pos.x = nsMath::MoveExp(pos.x, user.sceneX, CAMERA_MOVE_SPEED, g_frameTime);
        pos.y = nsMath::MoveExp(pos.y, user.sceneY, CAMERA_MOVE_SPEED, g_frameTime);
        origin.pos = pos;
    }
}

bool nsSceneView::OnPointerUp(float x, float y, int pointerId) {
    if (nsVisualContainer2d::OnPointerUp(x, y, pointerId)) {
        return true;
    }

    _dragging = false;
    return false;
}

bool nsSceneView::OnPointerDown(float x, float y, int pointerId) {
    if (nsVisualContainer2d::OnPointerDown(x, y, pointerId)) {
        return true;
    }

    _wheelAnchorActive = false;
    _dragging = true;
    _mousePos = {x, y};
    _mouseDown = {x, y};
    _startDragPos = origin.pos;
    return true;
}

bool nsSceneView::OnPointerMove(float x, float y, int pointerId) {
    if (nsVisualContainer2d::OnPointerMove(x, y, pointerId)) {
        return true;
    }

    _mousePos = {x, y};
    if (!_dragging && _wheelAnchorActive && _scene) {
        _wheelAnchorLocalPoint = origin.ToLocal(_mousePos);
    }

    if (_dragging) {
        const auto delta = nsVec2(x, y) - _mouseDown;
        const auto pos = _startDragPos + delta;
        auto &user = _appModel->project.user;
        user.sceneX = pos.x;
        user.sceneY = pos.y;
        return true;
    }

    return false;
}

bool nsSceneView::OnMouseWheel(float delta) {
    if (nsVisualContainer2d::OnMouseWheel(delta)) {
        return true;
    }

    auto &user = _appModel->project.user;
    const float zoom = std::fabs(origin.scale->x);
    const float newZoom = nsMath::Max(zoom + (zoom * WHEEL_ZOOM_STEP) * delta, MIN_ZOOM);

    _wheelAnchorLocalPoint = origin.ToLocal(_mousePos);
    _wheelAnchorActive = _scene != nullptr;
    user.zoom = newZoom;

    return true;
}

void nsSceneView::FitSceneToView() {
    if (!_scene) {
        return;
    }

    nsRect bounds;
    _scene->GetBounds(bounds, this);

    const float boundsWidth = bounds.maxX() - bounds.minX();
    const float boundsHeight = bounds.maxY() - bounds.minY();
    if (boundsWidth <= 0 || boundsHeight <= 0) {
        return;
    }

    const auto size = nsAppUtils::GetClientSize();
    constexpr float padding = 32.0f;
    const float viewWidth = std::max(size.x - padding * 2.0f, 1.0f);
    const float viewHeight = std::max(size.y - padding * 2.0f, 1.0f);
    const float targetZoom = nsMath::Max(std::min(viewWidth / boundsWidth, viewHeight / boundsHeight), MIN_ZOOM);

    FocusSceneBounds(targetZoom);
}

void nsSceneView::CenterSceneAt100() {
    FocusSceneBounds(1.0f);
}

bool nsSceneView::FocusSceneBounds(float targetZoom) {
    if (!_scene) {
        return false;
    }

    nsRect bounds;
    _scene->GetBounds(bounds, this);

    const float boundsWidth = bounds.maxX() - bounds.minX();
    const float boundsHeight = bounds.maxY() - bounds.minY();
    if (boundsWidth <= 0 || boundsHeight <= 0) {
        return false;
    }

    targetZoom = nsMath::Max(targetZoom, MIN_ZOOM);

    nsMatrix2 fitMatrix;
    fitMatrix.Transform({0, 0}, GetTargetScale(targetZoom), origin.angle);
    const auto size = nsAppUtils::GetClientSize();
    const nsVec2 viewCenter = size / 2.0f;
    const nsVec2 boundsCenter = bounds.GetCenter();
    const nsVec2 scaledCenter = fitMatrix.TransformPoint(boundsCenter);

    auto &user = _appModel->project.user;
    _wheelAnchorActive = false;
    user.sceneX = viewCenter.x - scaledCenter.x;
    user.sceneY = viewCenter.y - scaledCenter.y;
    user.zoom = targetZoom;
    return true;
}

nsVec2 nsSceneView::GetTargetScale(float zoom) const {
    auto &user = _appModel->project.user;
    return {
        (user.xFlip ? -1.0f : 1.0f) * zoom,
        (user.yFlip ? -1.0f : 1.0f) * zoom,
    };
}

void nsSceneView::DrawContent(const nsVisualContext2d &context) {
    constexpr float size = 10000.0f;
    RX_DrawLine({-size, 0}, {size, 0}, nsColor::green);
    RX_DrawLine({0, -size}, {0, size}, nsColor::red);

    nsVisualContainer2d::DrawContent(context);
}
