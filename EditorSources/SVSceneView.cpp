//
// Created by Roman on 10/21/2024.
//

#include "SVSceneView.h"
#include "nsLib/locator/ServiceLocator.h"
#include "Engine/TimeFormat.h"
#include "Core/Var.h"
#include "Engine/display/sprite/Sprite.h"

extern nsVar *sv_bg_color;

nsSVSceneView::nsSVSceneView() {
    _appModel = Locate<nsSVModel>();

    sv_bg_color->AddHandler(nsPropChangedName::CHANGED, [this](const nsBaseEvent *e) {
        UpdateBackColor();
    });
}

bool nsSVSceneView::ParseCustomProps(script_state_t *ss) {
    UpdateBackColor();
    return true;
}

void nsSVSceneView::Destroy() {
    if (_scene) {
        RemoveChild(_scene);
    }
    nsGroupLayout::Destroy();
}

void nsSVSceneView::SetScene(nsVisualObject2d *scene) {
    const auto m = _appModel;
    const auto &user = _appModel->user;

    if (_scene) {
        RemoveChild(_scene);
    }

    _scene = scene;
    if (_scene) {
        _scene->origin.scale = {
                user.xFlip ? -1 : 1 * m->zoom,
                user.yFlip ? -1 : 1 * m->zoom,
        };
        AddChild(_scene);
    }
}

void nsSVSceneView::Loop() {
    if (_scene) {

        auto &t = _scene->origin;
        auto m = _appModel;
        auto &user = _appModel->user;

        nsVec2 targetScale = {
                (user.xFlip ? -1.0f : 1.0f) * m->zoom,
                (user.yFlip ? -1.0f : 1.0f) * m->zoom,
        };

        nsVec2 scale = t.scale;

        t.scale = {
                nsMath::MoveExp(scale.x, targetScale.x, 10, g_frameTime),
                nsMath::MoveExp(scale.y, targetScale.y, 10, g_frameTime)
        };
    }

    nsVisualContainer2d::Loop();
}

void nsSVSceneView::DrawContent(const nsVisualContext2d &context) {
    if (_scene) {
        auto coords = GetChildById("coordSystem");
        coords->origin.pos = (nsVec2) _scene->origin.pos;
    }

    nsBaseLayout::DrawContent(context);
}

void nsSVSceneView::UpdateBackColor() {
    auto back = dynamic_cast<nsSprite*>(GetChildByIdRecursive("background"));
    if (back) {
        nsColor c;
        sscanf(sv_bg_color->String(), "%f %f %f %f", &c.r, &c.g, &c.b, &c.a);
        back->desc.color = c;
    }
}


