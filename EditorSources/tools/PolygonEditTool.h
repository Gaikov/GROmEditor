// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file PolygonEditTool.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "ITool.h"
#include "Engine/RenDevice.h"
#include "Engine/renderer/sprites/SpriteDesc.h"
#include "models/AppModel.h"

class nsVisualPolygon;

class nsPolygonEditTool : public ITool {
public:
    explicit nsPolygonEditTool(nsVisualPolygon *polygon);

    bool CanEdit() const override { return _polygon != nullptr; }
    const char *GetName() const override { return "polygon-edit"; }

    bool OnPointerDown(nsVisualObject2d *scene, float x, float y, int pointerId) override;
    bool OnPointerMove(nsVisualObject2d *scene, float x, float y, int pointerId) override;
    bool OnPointerUp(nsVisualObject2d *scene, float x, float y, int pointerId) override;
    bool OnKeyDown(int key, bool repeated, int mods) override;
    void DrawOverlay() override;

private:
    int PickPoint(float x, float y);

    nsAppModel *_appModel;
    nsVisualPolygon *_polygon;
    IRenState *_handleRenState = nullptr;
    ITexture *_handleTex = nullptr;
    nsSpriteDesc _desc;
    int _dragIndex = -1;
    nsVec2 _dragPointOriginal;
    nsVec2 _dragOffset;

    static constexpr float POINT_RADIUS = 8.0f;
};
