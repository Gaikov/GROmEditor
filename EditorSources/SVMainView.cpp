//
// Created by Roman on 10/21/2024.
//

#include "SVMainView.h"
#include "Engine/TimeFormat.h"

void nsSVMainView::SetScene(nsVisualObject2d *scene) {
    auto view = dynamic_cast<nsVisualContainer2d*>(GetChildById("sceneView"));

    if (_scene) {
        view->RemoveChild(_scene);
        _scene->Destroy();
    }

    _scene = scene;
    if (_scene) {
        view->AddChild(_scene);
    }
}


void nsSVMainView::Loop() {
    nsVisualContainer2d::Loop();

    if (_scene) {
        auto &t = _scene->origin;
        t.angle = nsMath::MoveExp(t.angle, _angle, 10, g_frameTime);

        nsVec2 pos = t.pos;
        pos.x = nsMath::MoveExp(pos.x, _targetPos.x, 50, g_frameTime);
        pos.y = nsMath::MoveExp(pos.y, _targetPos.y, 50, g_frameTime);
        t.pos = pos;
    }
}

bool nsSVMainView::OnPointerUp(float x, float y, int pointerId) {
    if (nsVisualContainer2d::OnPointerUp(x, y, pointerId)) {
        return true;
    }

    _dragging = false;
    return false;
}

bool nsSVMainView::OnPointerDown(float x, float y, int pointerId) {
    if (nsVisualContainer2d::OnPointerDown(x, y, pointerId)) {
        return true;
    }

    if (_scene) {
        _dragging = true;
        _mouseDown = {x, y};
        _startDragPos = _scene->origin.pos;
        return true;
    }

    return false;
}

bool nsSVMainView::OnPointerMove(float x, float y, int pointerId) {
    if (nsVisualContainer2d::OnPointerMove(x, y, pointerId)) {
        return true;
    }

    if (_dragging) {
        auto delta = nsVec2(x, y) - _mouseDown;
        _targetPos = _startDragPos + delta;
        return true;
    }

    return false;
}

void nsSVMainView::OnMouseWheel(float delta) {
    nsVisualContainer2d::OnMouseWheel(delta);
    _angle += nsMath::Sign(delta) * nsMath::ToRad(10);
}


