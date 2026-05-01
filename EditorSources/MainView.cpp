//
// Created by Roman on 10/21/2024.
//

#include "MainView.h"
#include "Engine/TimeFormat.h"
#include "SceneView.h"
#include "Engine/utils/AppUtils.h"
#include "nsLib/locator/ServiceLocator.h"
#include "scene/SceneUtils.h"
#include <cmath>

nsMainView::nsMainView() {
    _appModel = Locate<nsAppModel>();
    auto &p = _appModel->project;
    auto &user = _appModel->project.user;

    user.currentScene.AddHandler(nsPropChangedName::CHANGED, [&](const nsBaseEvent *) {
        SetScene(p.scenes.Get(user.currentScene));
    });

    user.backColor.AddHandler(nsPropChangedName::CHANGED, [&](const nsBaseEvent *) {
        _back->desc.color = user.backColor;
    });
    _sceneView = new nsSceneView();
    _back = new nsSprite();
    _back->desc.size = {100, 100};
    _back->desc.color = user.backColor;
    _back->renState = _device->StateLoad("default/rs/gui.ggrs");
    _back->desc.tex = _device->TextureLoad("default/editor/back.jpg");
}

void nsMainView::SetScene(nsVisualObject2d *scene) {
    _particles.clear();
    _wheelAnchorActive = false;

    _scene = scene;
    _sceneView->SetScene(scene);

    if (_scene) {
        if (auto container = dynamic_cast<nsVisualContainer2d *>(_scene)) {
            container->GetChildrenRecursive(_particles);
        }
    }
}

void nsMainView::OnAddedToStage() {
    nsVisualContainer2d::OnAddedToStage();
    AddChild(_back);
    AddChild(_sceneView);
}

void nsMainView::Loop() {
    auto &user = _appModel->project.user;
    const auto size = nsAppUtils::GetClientSize();

    _back->desc.size = size;
    _back->desc.tex2 = size / (user.backCellSize * 2.0f);

    auto &t = _sceneView->origin;
    t.angle = nsMath::MoveExp(t.angle, _angle, 10, g_frameTime);

    if (!_wheelAnchorActive || !_scene) {
        nsVec2 pos = t.pos;
        pos.x = nsMath::MoveExp(pos.x, user.sceneX, 50, g_frameTime);
        pos.y = nsMath::MoveExp(pos.y, user.sceneY, 50, g_frameTime);
        t.pos = pos;
    }

    nsVisualContainer2d::Loop();

    if (_wheelAnchorActive && _scene) {
        nsTransform2 anchorTransform;
        anchorTransform.pos = t.pos;
        anchorTransform.scale = t.scale;
        anchorTransform.angle = t.angle;

        const nsVec2 anchorGlobalPoint = anchorTransform.ToGlobal(_wheelAnchorLocalPoint);
        const nsVec2 deltaPos = _mousePos - anchorGlobalPoint;
        t.pos = t.pos + deltaPos;

        user.sceneX = t.pos->x;
        user.sceneY = t.pos->y;

        const nsVec2 targetScale = {
            (user.xFlip ? -1.0f : 1.0f) * user.zoom,
            (user.yFlip ? -1.0f : 1.0f) * user.zoom,
        };

        constexpr float scaleEps = 0.001f;
        if (std::fabs(t.scale->x - targetScale.x) < scaleEps
            && std::fabs(t.scale->y - targetScale.y) < scaleEps) {
            _wheelAnchorActive = false;
        }
    }
}

void nsMainView::DrawNode(const nsVisualContext2d &context) {
    nsVisualContainer2d::DrawNode(context);

    ApplyWorldMatrix();

    _device->StencilApply(nullptr);
    nsVisualObject2d *selected = _appModel->project.user.selectedObject;
    nsSceneUtils::DrawBounds(selected);
    nsSceneUtils::DrawOrigin(selected);
}

bool nsMainView::OnPointerUp(float x, float y, int pointerId) {
    if (nsVisualContainer2d::OnPointerUp(x, y, pointerId)) {
        return true;
    }

    _dragging = false;
    return false;
}

bool nsMainView::OnPointerDown(float x, float y, int pointerId) {
    if (nsVisualContainer2d::OnPointerDown(x, y, pointerId)) {
        return true;
    }


    _wheelAnchorActive = false;
    _dragging = true;
    _mousePos = {x, y};
    _mouseDown = {x, y};
    _startDragPos = _sceneView->origin.pos;
    return true;
}

bool nsMainView::OnPointerMove(float x, float y, int pointerId) {
    if (nsVisualContainer2d::OnPointerMove(x, y, pointerId)) {
        return true;
    }

    _mousePos = {x, y};
    if (!_dragging && _wheelAnchorActive && _scene) {
        _wheelAnchorLocalPoint = _sceneView->origin.ToLocal(_mousePos);
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

bool nsMainView::OnMouseWheel(float delta) {
    if (nsVisualContainer2d::OnMouseWheel(delta)) {
        return true;
    }

    auto &user = _appModel->project.user;
    const float zoom = std::fabs(_sceneView->origin.scale->x);
    const float newZoom = nsMath::Max(zoom + (zoom * 0.05f) * delta, 0.01f);

    _wheelAnchorLocalPoint = _sceneView->origin.ToLocal(_mousePos);
    _wheelAnchorActive = _scene != nullptr;
    user.zoom = newZoom;

    return true;
}


