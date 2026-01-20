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
    _appModel = Locate<nsAppModel>();
    auto &p = _appModel->project;

    _appModel->blastParticles.AddHandler(nsPropChangedName::CHANGED, [this](const nsBaseEvent *) {
        BlastParticles();
    });

    auto &user = _appModel->project.user;

    user.currentScene.AddHandler(nsPropChangedName::CHANGED, [&](const nsBaseEvent *) {
        SetScene(p.scenes.Get(user.currentScene));
    });

    user.backColor.AddHandler(nsPropChangedName::CHANGED, [&](const nsBaseEvent *) {
        _back->desc.color = user.backColor;
    });
    _sceneView = new nsSVSceneView();
    _back = new nsSprite();
    _back->desc.size = {100, 100};
    _back->desc.color = user.backColor;
}

void nsSVMainView::SetScene(nsVisualObject2d *scene) {
    _particles.clear();

    _scene = scene;
    _sceneView->SetScene(scene);

    if (_scene) {
        if (auto container = dynamic_cast<nsVisualContainer2d *>(_scene)) {
            container->GetChildrenRecursive(_particles);
        }
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

    const auto &user = _appModel->project.user;
    nsVec2 pos = t.pos;
    pos.x = nsMath::MoveExp(pos.x, user.sceneX, 50, g_frameTime);
    pos.y = nsMath::MoveExp(pos.y, user.sceneY, 50, g_frameTime);
    t.pos = pos;

    nsVisualContainer2d::Loop();
}

void nsSVMainView::DrawNode(const nsVisualContext2d &context) {
    nsVisualContainer2d::DrawNode(context);

    ApplyWorldMatrix();

    _device->StencilApply(nullptr);
    nsVisualObject2d *selected = _appModel->project.user.selectedObject;
    nsSceneUtils::DrawBounds(selected);
    nsSceneUtils::DrawOrigin(selected);
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
        auto &user = _appModel->project.user;
        user.sceneX = pos.x;
        user.sceneY = pos.y;
        return true;
    }

    return false;
}

bool nsSVMainView::OnMouseWheel(float delta) {
    nsVisualContainer2d::OnMouseWheel(delta);
    auto &user = _appModel->project.user;
    const float zoom = user.zoom;
    user.zoom = zoom + (zoom * 0.05f) * delta;
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
