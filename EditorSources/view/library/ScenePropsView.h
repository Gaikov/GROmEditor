// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ScenePropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "props/BaseVisualPropsView.h"
#include "view/BaseView.h"

class nsScenePropsView : public nsBaseView {
public:
    nsScenePropsView();

protected:
    void Draw() override;
    void DrawNode(nsVisualObject2d *node, int index);

private:
    nsVisualObject2d *_selected = nullptr;
    nsVisualObject2d *_scene = nullptr;

    std::vector<nsBaseVisualPropsView::sp_t> _propsViews;
};
