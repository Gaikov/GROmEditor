// Copyright (c) 2003-2024, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file SVMainView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "Engine/display/layouts/GroupLayout.h"

class nsSVMainView : public nsGroupLayout {
public:
    void SetScene(nsVisualObject2d *scene);

    void Loop() override;
protected:
    bool OnPointerUp(float x, float y, int pointerId) override;
    bool OnPointerDown(float x, float y, int pointerId) override;
    bool OnPointerMove(float x, float y, int pointerId) override;
    void OnMouseWheel(float delta) override;

private:
    nsVisualObject2d    *_scene;

    float   _angle = 0;

    bool _dragging = false;
    nsVec2  _mouseDown;
    nsVec2  _startDragPos;
    nsVec2  _targetPos;
};