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
#include "Engine/display/factory/VisualFactory2d.h"
#include "Engine/Input.h"
#include "Engine/renderer/particles/ParticlesManager.h"
#include "Core/sys.h"
#include "nsLib/locator/ServiceLocator.h"
#include "SVModel.h"
#include "SVSceneView.h"
#include "ThirdParty/imgui/ImGUI_gles3.h"
#include "view/TestView.h"
#include "view/ViewsRoot.h"

#define VIEWER_VERSION "SceneViewer 1.0.0-dev.0"
#define VIEWER_APP "GROm Scene Viewer"

static nsSceneViewerApp    g_sceneViewer;
static nsVar    *sv_last_layout = nullptr;
nsVar    *sv_bg_color = nullptr;

IGameApp*	App_GetGame() {
    return &g_sceneViewer;
}

bool nsSceneViewerApp::InitDialog() {
    return true;
}

bool nsSceneViewerApp::Init() {
    Log::Info("################### Init SceneViewer ###################");
    _guiBackend.Init(App_GetPlatform()->GetWindowHandler());
    nsViewsRoot::Init();
    nsViewsRoot::Shared()->AddView<nsTestView>();

    g_cfg->RegCmd("sv_unload", [this](int, const char*[]) {
        _root->SetScene(nullptr);
    });
    sv_bg_color = g_cfg->RegVar("sv_bg_color", "0.1 0.1 0.1 1", GVF_SAVABLE);

    nsServiceLocator::Init();
    auto locator = nsServiceLocator::Shared();
    locator->MapClass<nsSVModel>().AsSingleton();

    g_inp.ShowCursor(true);
    App_GetPlatform()->SetAppTitle(VIEWER_APP);
    _device = nsRenDevice::Shared()->Device();

    auto factory = nsVisualFactory2d::Shared();
    factory->BindClass<nsSVMainView>("MainView");
    factory->BindClass<nsSVSceneView>("SceneView");

    auto view = factory->Create("default/viewer/main.layout");
    _root = dynamic_cast<nsSVMainView *>(view);
    if (!_root) {
        Sys_FatalError("Invalid main layout!");
        return false;
    }
    _root->Prepare();

    g_cfg->RegCmd("sv_load", [this](int argc, const char *argv[] ) {
        if (argc > 1) {
            LoadLayout(argv[1]);
        } else {
            Log::Warning("usages: sv_load [layout path]");
        }
    });

    ReloadLayout();

    _appInput.AddInput(_root);
    _appInput.AddInput(this);
    _appInput._pointerTransform = &_ortho;
    _inputHandler.AddInput(&_guiBackend);
    _inputHandler.AddInput(&_appInput);

    return true;
}

void nsSceneViewerApp::Release() {
    Log::Info("################### Release SceneViewer ###################");
    if (_root) {
        _root->Destroy();
    }
    nsServiceLocator::Release();
    nsViewsRoot::Release();
    _guiBackend.Shutdown();
}

void nsSceneViewerApp::DrawWorld() {
    auto size = nsAppUtils::GetClientSize();

    _device->ClearScene(0xff);

    _ortho.SetScreenSize(size.x, size.y);
    _device->LoadProjMatrix(_ortho.GetViewMatrix());

    _root->SetWidth(size.x);
    _root->SetHeight(size.y);

    nsVisualSceneRender2d::DrawScene(_root);

    _guiBackend.StartFrame();
    nsViewsRoot::Shared()->Draw();
    _guiBackend.EndFrame();
}

void nsSceneViewerApp::Loop(float frameTime) {
    _root->Loop();
}

void nsSceneViewerApp::OnActivate(bool active) {
    if (active) {
        ReloadLayout();
        nsParticlesManager::Shared()->ReloadParticles();
    }
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

void nsSceneViewerApp::OnKeyUp(int key) {

}

void nsSceneViewerApp::OnKeyDown(int key, bool rept) {

}

void nsSceneViewerApp::OnChar(char ch) {

}

void nsSceneViewerApp::OnMouseWheel(float delta) {

}

void nsSceneViewerApp::LoadLayout(const char *filePath) {
    auto layout = nsVisualFactory2d::Shared()->Create(filePath);
    if (layout) {
        _root->SetScene(layout);
        sv_last_layout->SetString(filePath);
    }
}

void nsSceneViewerApp::ReloadLayout() {
    sv_last_layout = g_cfg->RegVar("sv_last_layout", "", GVF_SAVABLE);
    if (StrCheck(sv_last_layout->String())) {
        LoadLayout(sv_last_layout->String());
    }
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
