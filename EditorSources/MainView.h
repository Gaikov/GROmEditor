// Copyright (c) 2003-2024, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file SVMainView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "SceneView.h"
#include "Engine/display/layouts/GroupLayout.h"
#include "models/AppModel.h"
#include "Engine/display/particles/VisualParticles.h"
#include "Engine/display/sprite/Sprite.h"

class nsMainView : public nsVisualContainer2d {
public:
    nsMainView();
    void SetScene(nsVisualObject2d *scene);
    void Loop() override;
    void DrawNode(const nsVisualContext2d &context) override;

protected:
    void OnAddedToStage() override;
    bool OnPointerUp(float x, float y, int pointerId) override;
    bool OnPointerDown(float x, float y, int pointerId) override;
    bool OnPointerMove(float x, float y, int pointerId) override;
    bool OnMouseWheel(float delta) override;

private:
    nsAppModel           *_appModel;
    nsSceneView       *_sceneView;
    nsVisualObject2d    *_scene = nullptr;
    nsSprite *_back = nullptr;

    std::vector<nsVisualParticles*>  _particles;

    float   _angle = 0;

    bool _dragging = false;
    nsVec2  _mouseDown;
    nsVec2  _startDragPos;
};
