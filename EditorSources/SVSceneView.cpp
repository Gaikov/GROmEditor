//
// Created by Roman on 10/21/2024.
//

#include "SVSceneView.h"

void nsSVSceneView::SetScene(nsVisualObject2d *scene) {
    if (_scene) {
        RemoveChild(_scene);
        _scene->Destroy();
    }

    _scene = scene;
    if (_scene) {
        AddChild(_scene);
    }
}

void nsSVSceneView::DrawContent(const nsVisualContext2d &context) {
    if (_scene) {
        auto coords = GetChildById("coordSystem");
        coords->origin.pos = (nsVec2)_scene->origin.pos;
    }

    nsBaseLayout::DrawContent(context);
}
