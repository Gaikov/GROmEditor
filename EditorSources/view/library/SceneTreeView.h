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
    enum class DropMode {
        BEFORE,
        INSIDE,
        AFTER
    };

    nsVisualObject2d * _preselect;
    nsVisualObject2d * _scene;
    nsString _currentSceneRelativePath;

    void UpdateCurrentScenePath();
    void DrawDragDrop(nsVisualObject2d *node);
    void DrawDropPreview(DropMode mode) const;
    DropMode GetDropMode(nsVisualObject2d *node) const;
    bool GetDropTarget(nsVisualObject2d *target, DropMode mode, nsVisualContainer2d *&parent, int &index) const;
    void OnDragDrop(nsVisualObject2d *source, nsVisualObject2d *target, DropMode mode);
};
