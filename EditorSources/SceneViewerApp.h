// Copyright (c) 2003-2024, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file SceneViewerApp.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "Core/AppInfo.h"
#include "Engine/GameApp.h"
#include "Engine/display/layouts/GroupLayout.h"
#include "nsLib/math/ortho/OrthogonalNativeView.h"

class nsSceneViewerApp : public IGameApp, IUserInput {
public:
    bool InitDialog() override;
    bool Init() override;
    void Release() override;
    void DrawWorld() override;
    void Loop(float frameTime) override;
    void OnActivate(bool active) override;
    void OnPause(bool paused) override;
    int GetWindowIcon() override;
    IUserInput *GetUserInput() override;
    void GetGUIDimension(int &width, int &height) override;
    const char *GetVersionInfo() override;

private:
    bool OnPointerUp(float x, float y, int pointerId) override;
    bool OnPointerDown(float x, float y, int pointerId) override;
    bool OnPointerMove(float x, float y, int pointerId) override;
    void OnPointerCancel(int pointerId) override;
    void OnKeyUp(int key) override;
    void OnKeyDown(int key, bool rept) override;
    void OnChar(char ch) override;
    void OnMouseWheel(float delta) override;

    void LoadLayout(const char *filePath);
    void ReloadLayout();
private:
    nsVisualObject2d    *_layout;
    nsGroupLayout   *_root;
    IRenDevice      *_device;
    nsOrthogonalNativeView  _ortho;
    float   _angle = 0;

    bool _dragging = false;
    nsVec2  _mouseDown;
    nsVec2  _startDragPos;
    nsVec2  _targetPos;
};