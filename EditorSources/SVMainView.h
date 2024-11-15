// Copyright (c) 2003-2024, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file SVMainView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "Engine/display/layouts/GroupLayout.h"
#include "SVModel.h"
#include "Engine/display/particles/VisualParticles.h"

class nsSVMainView : public nsGroupLayout {
public:
    nsSVMainView();
    bool Prepare();
    void SetScene(nsVisualObject2d *scene);

    void Loop() override;
protected:
    bool OnPointerUp(float x, float y, int pointerId) override;
    bool OnPointerDown(float x, float y, int pointerId) override;
    bool OnPointerMove(float x, float y, int pointerId) override;
    void OnMouseWheel(float delta) override;

    void EmitParticles(bool emit);
    void BlastParticles();

private:
    nsSVModel           *_appModel;
    nsVisualObject2d    *_scene = nullptr;

    std::vector<nsVisualParticles*>  _particles;

    float   _angle = 0;

    bool _dragging = false;
    nsVec2  _mouseDown;
    nsVec2  _startDragPos;
    nsVec2  _targetPos;
};