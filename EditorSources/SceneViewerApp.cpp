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
#include "models/SVModel.h"
#include "SVSceneView.h"
#include "Core/undo/UndoService.h"
#include "ThirdParty/imgui/ImGUI_gles3.h"
#include "view/ViewsRoot.h"
#include "view/popups/PopupsStack.h"

#define VIEWER_VERSION "SceneViewer 1.0.0-dev.0"
#define VIEWER_APP "GROm Scene Viewer"

static nsSceneViewerApp    g_sceneViewer;
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

    g_cfg->RegCmd("sv_unload", [this](int, const char*[]) {
        _root->SetScene(nullptr);
    });
    sv_bg_color = g_cfg->RegVar("sv_bg_color", "0.1 0.1 0.1 1", GVF_SAVABLE);

    nsUndoService::Init();
    nsServiceLocator::Init();
    auto locator = nsServiceLocator::Shared();
    locator->MapClass<nsSVModel>().AsSingleton().GetInstance();
    _appModel = Locate<nsSVModel>();

    g_inp.ShowCursor(true);
    App_GetPlatform()->SetAppTitle(VIEWER_APP);
    _device = nsRenDevice::Shared()->Device();
    nsViewsRoot::Init();
    nsPopupsStack::Init();

    auto factory = nsVisualFactory2d::Shared();
    factory->BindClass<nsSVMainView>("MainView");
    factory->BindClass<nsSVSceneView>("SceneView");

    auto view = factory->Create("default/viewer/main.layout");
    _root = dynamic_cast<nsSVMainView *>(view);
    if (!_root) {
        Sys_FatalError("Invalid main layout!");
        return false;
    }

    g_cfg->RegCmd("sv_load", [this](int argc, const char *argv[] ) {
        if (argc > 1) {
            LoadLayout(argv[1]);
        } else {
            Log::Warning("usages: sv_load [layout path]");
        }
    });

    _appModel->project.currentScene.AddHandler(nsPropChangedName::CHANGED, [this](const nsBaseEvent*) {
        ReloadLayout();
    });

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
    nsPopupsStack::Release();
    nsViewsRoot::Release();
    nsServiceLocator::Release();
    nsUndoService::Release();
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
    nsPopupsStack::Shared()->Draw();
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

bool nsSceneViewerApp::OnMouseWheel(float delta) {
    return false;
}

void nsSceneViewerApp::LoadLayout(const char *filePath) {
    const auto layout = Locate<nsSVModel>()->project.scenes.Get(filePath);
    if (layout) {
        _root->SetScene(layout);
        _appModel->project.currentScene = filePath;
    }
}

void nsSceneViewerApp::ReloadLayout() {
    if (!_appModel->project.currentScene->empty()) {
        LoadLayout(_appModel->project.currentScene->c_str());
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
