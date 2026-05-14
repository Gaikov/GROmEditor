// Copyright (c) 2003-2024, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file SVSceneView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "gizmos/VisualHolder.h"
#include "models/AppModel.h"

class nsSceneView : public nsVisualHolder {
public:
    static constexpr float CAMERA_MOVE_SPEED = 20.0f;
    static constexpr float CAMERA_ANGLE_SPEED = 10.0f;
    static constexpr float MIN_ZOOM = 0.01f;
    static constexpr float WHEEL_ZOOM_STEP = 0.1f;
    static constexpr float ANCHOR_SCALE_EPS = 0.001f;

    nsSceneView();

    void SetScene(nsVisualObject2d *scene);
    void Loop() override;
    void Destroy() override;

protected:
    void DrawContent(const nsVisualContext2d &context) override;
    bool OnPointerUp(float x, float y, int pointerId) override;
    bool OnPointerDown(float x, float y, int pointerId) override;
    bool OnPointerMove(float x, float y, int pointerId) override;
    bool OnMouseWheel(float delta) override;

private:
    void UpdateCamera();
    void FitSceneToView();
    void CenterSceneAt100();
    bool FocusSceneBounds(float targetZoom);
    bool FocusObjectBounds(nsVisualObject2d *target, float targetZoom, bool requireExtent);
    nsVisualObject2d *GetFocusTarget() const;
    nsVec2 GetTargetScale(float zoom) const;

    nsVisualObject2d    *_scene = nullptr;
    nsAppModel           *_appModel;

    float _targetAngle = 0;
    bool _dragging = false;
    bool _wheelAnchorActive = false;
    nsVec2 _mouseDown;
    nsVec2 _mousePos;
    nsVec2 _wheelAnchorLocalPoint;
    nsVec2 _startDragPos;
};
