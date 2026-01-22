//
// Created by Roman on 10/21/2024.
//

#include "SceneView.h"

#include "Engine/RenAux.h"
#include "nsLib/locator/ServiceLocator.h"
#include "Engine/TimeFormat.h"
#include "Engine/display/sprite/Sprite.h"

nsSceneView::nsSceneView() {
    _appModel = Locate<nsAppModel>();
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
    _scene = scene;
    if (_scene) {
        AddChild(_scene);
    }
}

void nsSceneView::Loop() {
    if (_scene) {
        auto &t = origin;
        auto &user = _appModel->project.user;

        nsVec2 targetScale = {
            (user.xFlip ? -1.0f : 1.0f) * user.zoom,
            (user.yFlip ? -1.0f : 1.0f) * user.zoom,
        };

        nsVec2 scale = t.scale;

        t.scale = {
            nsMath::MoveExp(scale.x, targetScale.x, 10, g_frameTime),
            nsMath::MoveExp(scale.y, targetScale.y, 10, g_frameTime)
        };
    }

    nsVisualContainer2d::Loop();
}

void nsSceneView::DrawContent(const nsVisualContext2d &context) {
    constexpr float size = 10000.0f;
    RX_DrawLine({-size, 0}, {size, 0}, nsColor::green);
    RX_DrawLine({0, -size}, {0, size}, nsColor::red);

    nsVisualContainer2d::DrawContent(context);
}
