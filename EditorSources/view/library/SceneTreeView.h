// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file SceneTreeView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "view/BaseView.h"

class nsSceneTreeView : public nsBaseView {
public:
    nsSceneTreeView();

protected:
    void Draw() override;
    void DrawNode(nsVisualObject2d *node, int index);
    void PostDraw();

private:
    nsVisualObject2d * _preselect;
    nsVisualObject2d * _scene;
};
