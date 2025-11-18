// Copyright (c) 2003-2024, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file SVSceneView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "Engine/display/layouts/GroupLayout.h"
#include "models/SVModel.h"
#include "Engine/display/factory/CustomVisual.h"

class nsSVSceneView : public nsGroupLayout, public nsCustomVisual {
public:
    nsSVSceneView();

    void SetScene(nsVisualObject2d *scene);
    void Loop() override;
    bool ParseCustomProps(script_state_t *ss) override;
    void Destroy() override;

protected:
    void DrawContent(const nsVisualContext2d &context) override;
    void UpdateBackColor();

private:
    nsVisualObject2d    *_scene = nullptr;
    nsSVModel           *_appModel;
};