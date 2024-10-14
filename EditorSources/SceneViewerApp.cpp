//
// Created by Roman on 10/14/2024.
//

#include "SceneViewerApp.h"
#include "Engine/Platform.h"
#include "Engine/display/VisualSceneRender2d.h"
#include "Engine/RenManager.h"
#include "nsLib/log.h"
#include "Engine/utils/AppUtils.h"
#include "Engine/display/factory/VisualFactory2d.h"

#define VIEWER_VERSION "SceneViewer 1.0.0-dev.0"
#define VIEWER_APP "GROm Scene Viewer"

static nsSceneViewerApp    g_sceneViewer;

IGameApp*	App_GetGame() {
    return &g_sceneViewer;
}

bool nsSceneViewerApp::InitDialog() {
    return true;
}

bool nsSceneViewerApp::Init() {
    Log::Info("################### Init SceneViewer ###################");
    App_GetPlatform()->SetAppTitle(VIEWER_APP);
    _device = nsRenDevice::Shared()->Device();
    _root = new nsGroupLayout();

    auto layout = nsVisualFactory2d::Shared()->Create("tests/layouts/container.layout");
    if (layout) {
        _root->AddChild(layout);
    }
    return true;
}

void nsSceneViewerApp::Release() {
    Log::Info("################### Release SceneViewer ###################");
    _root->Destroy();
}

void nsSceneViewerApp::DrawWorld() {
    _device->ClearScene(0xff);

    auto size = nsAppUtils::GetClientSize();
    _root->SetWidth(size.x);
    _root->SetHeight(size.y);

    nsVisualSceneRender2d::DrawScene(_root);
}

void nsSceneViewerApp::Loop(float frameTime) {
    _root->Loop();
}

void nsSceneViewerApp::OnActivate(bool active) {

}

void nsSceneViewerApp::OnPause(bool paused) {

}

int nsSceneViewerApp::GetWindowIcon() {
    return 0;
}

IUserInput *nsSceneViewerApp::GetUserInput() {
    return nullptr;
}

void nsSceneViewerApp::GetGUIDimension(int &width, int &height) {
    App_GetPlatform()->GetClientSize(width, height);
}

const char *nsSceneViewerApp::GetVersionInfo() {
    return VIEWER_VERSION;
}


class nsSceneViewerInfo : public IAppInfo {
public:
    const char *GetAppName() override {
        return "scene_viewer";
    }
    const char *GetDefCfgPath() override {
        return nullptr;
    }
};

static nsSceneViewerInfo g_sceneAppInfo;

IAppInfo* App_GetInfo() {
    return &g_sceneAppInfo;
}
