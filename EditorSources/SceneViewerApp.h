// Copyright (c) 2003-2024, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file SceneViewerApp.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "Engine/GameApp.h"
#include "nsLib/math/ortho/OrthogonalNativeView.h"
#include "Engine/input/MultiUserInput.h"
#include "SVMainView.h"
#include "imgui/ImGUI_gles3.h"

class nsSceneViewerApp : public IGameApp, public IUserInput {
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
    void OnKeyUp(int key, int mods) override;
    void OnKeyDown(int key, bool rept, int mods) override;
    void OnChar(char ch) override;
    bool OnMouseWheel(float delta) override;

    void LoadLayout(const char *filePath);
    void ReloadLayout();

private:
    nsSVModel      *_appModel = nullptr;
    nsVisualContainer2d *_stage = nullptr;
    nsSVMainView   *_view = nullptr;
    IRenDevice      *_device = nullptr;
    nsOrthogonalNativeView  _ortho;

    nsMultiUserInput    _appInput;
    nsMultiUserInput    _inputHandler;
    nsImGUI_gles3       _guiBackend;
};
