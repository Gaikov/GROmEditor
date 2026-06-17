// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file FreeTransformTool.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "ITool.h"
#include "Engine/RenDevice.h"
#include "Engine/renderer/sprites/SpriteDesc.h"
#include "models/AppModel.h"

class FreeTransformTool : public ITool {
public:
    FreeTransformTool();

    bool CanEdit() const override { return _appModel->project.user.selectedObject.GetValue() != nullptr; }
    const char *GetName() const override { return "free-transform"; }

    bool OnPointerDown(nsVisualObject2d *scene, float x, float y, int pointerId) override;
    bool OnPointerMove(nsVisualObject2d *scene, float x, float y, int pointerId) override;
    bool OnPointerUp(nsVisualObject2d *scene, float x, float y, int pointerId) override;
    bool OnKeyDown(int key, bool repeated, int mods) override;
    void DrawOverlay() override;

private:
    enum Mode { NONE, MOVE, ROTATE };
    static constexpr float HANDLE_RADIUS = 20.0f;

    nsVisualObject2d *GetTarget() const { return _appModel->project.user.selectedObject.GetValue(); }
    int FindCorner(float x, float y);
    nsVec2 GetCornerWorld(int index) const;

    nsAppModel *_appModel;

    IRenState *_handleRenState = nullptr;
    ITexture *_handleTex = nullptr;
    nsSpriteDesc _desc;
    nsTransform2 _transform;

    Mode _mode = NONE;
    int _dragHandle = -1;
    int _hoverHandle = -1;
    nsVec2 _startPos;
    float _startAngle = 0;
    nsVec2 _mouseOffset;
    float _startMouseAngle = 0;
};
