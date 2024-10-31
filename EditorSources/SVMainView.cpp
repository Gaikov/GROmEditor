//
// Created by Roman on 10/21/2024.
//

#include "SVMainView.h"

#include "SVUtils.h"
#include "Engine/TimeFormat.h"
#include "nsLib/log.h"
#include "Engine/display/particles/VisualParticles.h"
#include "SVSceneView.h"
#include "nsLib/locator/ServiceLocator.h"

nsSVMainView::nsSVMainView() {
    _appModel = Locate<nsSVModel>();
}

bool nsSVMainView::Prepare() {
    nsSVUtils::GetButton(this, "buttonEmit")->SetClickHandler([this] {
        Log::Info("Emit");
        EmitParticles(!_emitParticles);
    });

    nsSVUtils::GetButton(this, "buttonBlast")->SetClickHandler([this]{
        Log::Info("Blast");
        BlastParticles();
    });

    nsSVUtils::GetButton(this, "buttonXFlip")->SetClickHandler([this] {
        _appModel->xFlip = !_appModel->xFlip.GetValue();
    });

    nsSVUtils::GetButton(this, "buttonYFlip")->SetClickHandler([this] {
        _appModel->yFlip = !_appModel->yFlip.GetValue();
    });

    nsSVUtils::OnButtonClick(this, "buttonResetZoom", [this] {
        _appModel->zoom = 1;
    });

    return true;
}

void nsSVMainView::SetScene(nsVisualObject2d *scene) {
    auto view = dynamic_cast<nsSVSceneView*>(GetChildById("sceneView"));
    _particles.clear();

    _scene = scene;
    view->SetScene(scene);

    if (_scene) {
        if (auto container = dynamic_cast<nsVisualContainer2d*>(_scene)) {
            container->FindChildrenRecursive([](nsVisualObject2d *child) -> bool {
                return dynamic_cast<nsVisualParticles*>(child);
            }, _particles);
        }
        EmitParticles(true);
    }
}


void nsSVMainView::Loop() {
    if (_scene) {
        auto &t = _scene->origin;
        t.angle = nsMath::MoveExp(t.angle, _angle, 10, g_frameTime);

        nsVec2 pos = t.pos;
        pos.x = nsMath::MoveExp(pos.x, _targetPos.x, 50, g_frameTime);
        pos.y = nsMath::MoveExp(pos.y, _targetPos.y, 50, g_frameTime);
        t.pos = pos;
    }

    nsVisualContainer2d::Loop();
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
    float zoom = _appModel->zoom;
    _appModel->zoom = zoom + (zoom * 0.05f) * delta;
    //_angle += nsMath::Sign(delta) * nsMath::ToRad(10);
}

void nsSVMainView::EmitParticles(bool emit) {
    _emitParticles = emit;
    for (auto p : _particles) {
        auto ps = dynamic_cast<nsVisualParticles*>(p);
        ps->GetSystem().spawnEnabled = emit;
    }
}

void nsSVMainView::BlastParticles() {
    for (auto p : _particles) {
        auto &ps = dynamic_cast<nsVisualParticles*>(p)->GetSystem();
        if (!ps.spawnEnabled) {
            ps.Emit();
        }
    }
}




