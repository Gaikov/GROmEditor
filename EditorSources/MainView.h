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

private:
    nsAppModel           *_appModel;
    nsSceneView       *_sceneView;
    nsVisualObject2d    *_scene = nullptr;
    nsSprite *_back = nullptr;

    std::vector<nsVisualParticles*>  _particles;
};
