//
// Created by Roman on 10/21/2024.
//

#include "SVMainView.h"
#include "Engine/TimeFormat.h"
#include "nsLib/log.h"
#include "SVSceneView.h"
#include "Engine/utils/AppUtils.h"
#include "nsLib/locator/ServiceLocator.h"
#include "scene/SceneUtils.h"

nsSVMainView::nsSVMainView() {
    _appModel = Locate<nsSVModel>();
    auto &p = _appModel->project;
    _appModel->user.emitParticles.AddHandler(nsPropChangedName::CHANGED, [this](const nsBaseEvent *) {
        EmitParticles(_appModel->user.emitParticles);
    });

    _appModel->blastParticles.AddHandler(nsPropChangedName::CHANGED, [this](const nsBaseEvent *) {
        BlastParticles();
    });

    _appModel->user.currentScene.AddHandler(nsPropChangedName::CHANGED, [&](const nsBaseEvent *) {
        SetScene(p.scenes.Get(_appModel->user.currentScene));
    });

    _appModel->user.backColor.AddHandler(nsPropChangedName::CHANGED, [this](const nsBaseEvent *) {
        _back->desc.color = _appModel->user.backColor;
    });
    _sceneView = new nsSVSceneView();
    _back = new nsSprite();
    _back->desc.size = {100, 100};
    _back->desc.color = _appModel->user.backColor;
}

void nsSVMainView::SetScene(nsVisualObject2d *scene) {
    _particles.clear();

    _scene = scene;
    _sceneView->SetScene(scene);

    if (_scene) {
        if (auto container = dynamic_cast<nsVisualContainer2d *>(_scene)) {
            container->GetChildrenRecursive(_particles);
        }
        EmitParticles(_appModel->user.emitParticles);
    }
}

void nsSVMainView::OnAddedToStage() {
    nsVisualContainer2d::OnAddedToStage();
    AddChild(_back);
    AddChild(_sceneView);
}

void nsSVMainView::Loop() {
    const auto size = nsAppUtils::GetClientSize();
    _back->desc.size = size;

    auto &t = _sceneView->origin;
    t.angle = nsMath::MoveExp(t.angle, _angle, 10, g_frameTime);

    auto &user = _appModel->user;
    nsVec2 pos = t.pos;
    pos.x = nsMath::MoveExp(pos.x, user.sceneX, 50, g_frameTime);
    pos.y = nsMath::MoveExp(pos.y, user.sceneY, 50, g_frameTime);
    t.pos = pos;

    nsVisualContainer2d::Loop();
}

void nsSVMainView::DrawNode(const nsVisualContext2d &context) {
    nsVisualContainer2d::DrawNode(context);

    ApplyWorldMatrix();
    nsSceneUtils::DrawBounds(_appModel->user.selectedObject);
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


    _dragging = true;
    _mouseDown = {x, y};
    _startDragPos = _sceneView->origin.pos;
    return true;
}

bool nsSVMainView::OnPointerMove(float x, float y, int pointerId) {
    if (nsVisualContainer2d::OnPointerMove(x, y, pointerId)) {
        return true;
    }

    if (_dragging) {
        const auto delta = nsVec2(x, y) - _mouseDown;
        const auto pos = _startDragPos + delta;
        auto &user = _appModel->user;
        user.sceneX = pos.x;
        user.sceneY = pos.y;
        return true;
    }

    return false;
}

bool nsSVMainView::OnMouseWheel(float delta) {
    nsVisualContainer2d::OnMouseWheel(delta);
    const float zoom = _appModel->user.zoom;
    _appModel->user.zoom = zoom + (zoom * 0.05f) * delta;
    return true;
}

void nsSVMainView::EmitParticles(bool emit) {
    Log::Info("emit: %i", emit ? 1 : 0);
    for (auto p: _particles) {
        p->GetSystem().spawnEnabled = emit;
    }
}

void nsSVMainView::BlastParticles() {
    for (auto p: _particles) {
        auto &ps = p->GetSystem();
        if (!ps.spawnEnabled) {
            ps.Emit();
        }
    }
}
