//
// Created by Roman on 10/14/2024.
//

#include "SceneViewerApp.h"
#include "Core/AppInfo.h"
#include "Engine/Platform.h"
#include "Engine/display/VisualSceneRender2d.h"
#include "Engine/RenManager.h"
#include "nsLib/log.h"
#include "Engine/utils/AppUtils.h"
#include "Engine/Input.h"
#include "nsLib/locator/ServiceLocator.h"
#include "models/SVModel.h"
#include "Core/undo/UndoService.h"
#include "display/VisualRefBuilder.h"
#include "display/lifecycle/VisualsLifecycle.h"
#include "Engine/display/factory/VisualFactory2d.h"
#include "ThirdParty/imgui/ImGUI_gles3.h"
#include "view/ViewsRoot.h"
#include "view/popups/PopupsStack.h"

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
    _guiBackend.Init(App_GetPlatform()->GetWindowHandler());

    nsUndoService::Init();

    nsServiceLocator::Init();
    auto locator = nsServiceLocator::Shared();
    locator->MapClass<nsSVModel>().AsSingleton().GetInstance();
    _appModel = Locate<nsSVModel>();

    nsVisualFactory2d::Shared()->RegisterBuilderWithName<nsVisualRefBuilder>();
    nsVisualsLifecycle::Init();

    g_inp.ShowCursor(true);
    App_GetPlatform()->SetAppTitle(VIEWER_APP);
    _device = nsRenDevice::Shared()->Device();
    nsViewsRoot::Init();
    nsPopupsStack::Init();

    _view = new nsSVMainView();
    _stage = nsVisualContainer2d::CreateStage();
    _stage->AddChild(_view);

    _appInput.AddInput(_stage);
    _appInput.AddInput(this);
    _appInput._pointerTransform = &_ortho;
    _inputHandler.AddInput(&_guiBackend);
    _inputHandler.AddInput(&_appInput);

    _appModel->Load();

    return true;
}

void nsSceneViewerApp::Release() {
    Log::Info("################### Release SceneViewer ###################");
    if (_stage) {
        _stage->Destroy();
    }

    _appModel->Save();

    nsPopupsStack::Release();
    nsViewsRoot::Release();
    nsVisualsLifecycle::Release();
    nsServiceLocator::Release();
    nsUndoService::Release();
    _guiBackend.Shutdown();
}

void nsSceneViewerApp::DrawWorld() {
    auto size = nsAppUtils::GetClientSize();

    _device->ClearScene(0xff);

    _ortho.SetScreenSize(size.x, size.y);
    _device->LoadProjMatrix(_ortho.GetViewMatrix());


    nsVisualSceneRender2d::DrawScene(_stage);

    _guiBackend.StartFrame();
    nsViewsRoot::Shared()->Draw();
    nsPopupsStack::Shared()->Draw();
    _guiBackend.EndFrame();
}

void nsSceneViewerApp::Loop(float frameTime) {
    _stage->Loop();
}

void nsSceneViewerApp::OnActivate(bool active) {
}

void nsSceneViewerApp::OnPause(bool paused) {

}

int nsSceneViewerApp::GetWindowIcon() {
    return 0;
}

IUserInput *nsSceneViewerApp::GetUserInput() {
    return &_inputHandler;
}

void nsSceneViewerApp::GetGUIDimension(int &width, int &height) {
    App_GetPlatform()->GetClientSize(width, height);
}

const char *nsSceneViewerApp::GetVersionInfo() {
    return VIEWER_VERSION;
}

bool nsSceneViewerApp::OnPointerUp(float x, float y, int pointerId) {
    return false;
}

bool nsSceneViewerApp::OnPointerDown(float x, float y, int pointerId) {
    return false;
}

bool nsSceneViewerApp::OnPointerMove(float x, float y, int pointerId) {
    return false;
}

void nsSceneViewerApp::OnPointerCancel(int pointerId) {

}

void nsSceneViewerApp::OnKeyUp(int key, int mods) {

}

void nsSceneViewerApp::OnKeyDown(int key, bool rept, int mods) {

}

void nsSceneViewerApp::OnChar(char ch) {

}

bool nsSceneViewerApp::OnMouseWheel(float delta) {
    return false;
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
