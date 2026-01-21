// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ScenePropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "view/BaseView.h"

class nsScenePropsView : public nsBaseView {
public:
    nsScenePropsView();

protected:
    void Draw() override;

private:
    nsVisualObject2d *_scene = nullptr;
};
