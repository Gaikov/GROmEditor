// Copyright (c) 2003-2024, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file SceneViewerApp.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "Core/AppInfo.h"
#include "Engine/GameApp.h"
#include "Engine/display/layouts/GroupLayout.h"

class nsSceneViewerApp : public IGameApp {
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
    nsGroupLayout   *_root;
    IRenDevice      *_device;
};