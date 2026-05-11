//
// Created by Roman on 10/21/2024.
//

#include "MainView.h"
#include "SceneView.h"
#include "Engine/utils/AppUtils.h"
#include "nsLib/locator/ServiceLocator.h"
#include "scene/SceneUtils.h"

nsMainView::nsMainView() {
    _appModel = Locate<nsAppModel>();
    auto &p = _appModel->project;
    auto &user = _appModel->project.user;

    user.currentScene.AddHandler(nsPropChangedName::CHANGED, [&](const nsBaseEvent *) {
        SetScene(p.scenes.Get(user.currentScene));
    });

    user.backColor.AddHandler(nsPropChangedName::CHANGED, [&](const nsBaseEvent *) {
        _back->desc.color = user.backColor;
    });
    _sceneView = new nsSceneView();
    _back = new nsSprite();
    _back->desc.size = {100, 100};
    _back->desc.color = user.backColor;
    _back->renState = _device->StateLoad("default/rs/gui.ggrs");
    _back->desc.tex = _device->TextureLoad("default/editor/back.jpg");
}

void nsMainView::SetScene(nsVisualObject2d *scene) {
    _particles.clear();

    _scene = scene;
    _sceneView->SetScene(scene);

    if (_scene) {
        if (auto container = dynamic_cast<nsVisualContainer2d *>(_scene)) {
            container->GetChildrenRecursive(_particles);
        }
    }
}

void nsMainView::OnAddedToStage() {
    nsVisualContainer2d::OnAddedToStage();
    AddChild(_back);
    AddChild(_sceneView);
}

void nsMainView::Loop() {
    auto &user = _appModel->project.user;
    const auto size = nsAppUtils::GetClientSize();

    _back->desc.size = size;
    _back->desc.tex2 = size / (user.backCellSize * 2.0f);

    nsVisualContainer2d::Loop();
}

void nsMainView::DrawNode(const nsVisualContext2d &context) {
    nsVisualContainer2d::DrawNode(context);

    ApplyWorldMatrix();

    _device->StencilApply(nullptr);
    nsVisualObject2d *selected = _appModel->project.user.selectedObject;
    nsSceneUtils::DrawBounds(selected);
    nsSceneUtils::DrawOrigin(selected);
}
