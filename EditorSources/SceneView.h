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
    nsSceneView();

    void SetScene(nsVisualObject2d *scene);
    void Loop() override;
    void Destroy() override;

protected:
    void DrawContent(const nsVisualContext2d &context) override;

private:
    nsVisualObject2d    *_scene = nullptr;
    nsAppModel           *_appModel;
};