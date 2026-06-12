// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file SelectionTool.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "ITool.h"
#include "Engine/display/VisualObject2d.h"
#include "models/AppModel.h"

class SelectionTool : public ITool {
public:
    SelectionTool();

    bool CanEdit() const override { return true; }
    const char *GetName() const override { return "selection"; }

    bool OnPointerDown(nsVisualObject2d *scene, float x, float y, int pointerId) override;
    bool OnPointerMove(nsVisualObject2d *scene, float x, float y, int pointerId) override;
    bool OnPointerUp(nsVisualObject2d *scene, float x, float y, int pointerId) override;

    void DrawOverlay() override;

private:
    nsVisualObject2d *PickObject(nsVisualObject2d *node, float x, float y);

    nsAppModel *_appModel;
    nsVisualObject2d *_hoverTarget = nullptr;
};
