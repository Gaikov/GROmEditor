// Copyright (c) 2003-2024, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file SVSceneView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "Engine/display/layouts/GroupLayout.h"

class nsSVSceneView : public nsGroupLayout {
public:
    void SetScene(nsVisualObject2d *scene);
protected:
    void DrawContent(const nsVisualContext2d &context) override;

private:
    nsVisualObject2d    *_scene = nullptr;
};