// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file LayoutAssetPolicy.cpp
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#include "LayoutAssetPolicy.h"

#include "Core/undo/UndoBatch.h"
#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVarChange.h"
#include "Engine/display/VisualType.h"
#include "Engine/display/VisualObject2d.h"
#include "Engine/display/container/VisualContainer2d.h"
#include "display/VisualRef.h"
#include "models/AppModel.h"
#include "nsLib/locator/ServiceLocator.h"
#include "nsLib/log.h"
#include "view/alerts/AlertPopup.h"
#include <string>

nsLayoutAssetPolicy::nsLayoutAssetPolicy() {
    _model = Locate<nsAppModel>();
}

void nsLayoutAssetPolicy::OnDoubleClick(const nsFilePath &path) {
    Log::Info("Selected: %s", path.AsChar());

    auto &user = _model->project.user;
    const std::string value = path.AsChar();
    const auto batch = new nsUndoBatch();

    batch->Add(new nsUndoVarChange(user.selectedObject,
                                   static_cast<nsVisualObject2d *>(nullptr)));
    batch->Add(new nsUndoVarChange(user.currentScene, value));

    nsUndoService::Shared()->Push(batch);
}

bool nsLayoutAssetPolicy::IsSelected(const nsFilePath &path) const {
    return _model->project.user.currentScene == path.AsChar();
}

nsVisualObject2d *nsLayoutAssetPolicy::CreateVisual(const nsFilePath &path) {
    auto &project = _model->project;
    const nsFilePath currentScene(project.user.currentScene->c_str());
    if (!currentScene.IsEmpty() && path == currentScene) {
        nsAlertPopup::Warning("Can't add scene to itself");
        return nullptr;
    }

    nsVisualObject2d *layout = project.scenes.Get(path.AsChar());
    if (!layout) {
        nsAlertPopup::Warning("Can't load layout");
        return nullptr;
    }

    if (!currentScene.IsEmpty() && ContainsSceneReference(layout, currentScene)) {
        nsAlertPopup::Warning("Can't add layout referencing current scene");
        return nullptr;
    }

    auto *visual = project.scenes.Create<nsVisualRef>(nsVisualType::LAYOUT_REF);
    if (!visual) {
        return nullptr;
    }

    visual->id = path.GetNameOnly().AsChar();
    visual->source = path.AsChar();
    return visual;
}

bool nsLayoutAssetPolicy::ContainsSceneReference(nsVisualObject2d *visual,
                                                 const nsFilePath &scenePath) const {
    if (!visual) {
        return false;
    }

    if (const auto ref = dynamic_cast<nsVisualRef *>(visual)) {
        const nsFilePath source(ref->source->c_str());
        if (!source.IsEmpty() && source == scenePath) {
            return true;
        }
        if (ContainsSceneReference(ref->GetRef(), scenePath)) {
            return true;
        }
    }

    if (const auto container = dynamic_cast<nsVisualContainer2d *>(visual)) {
        for (auto *child: container->GetChildren()) {
            if (ContainsSceneReference(child, scenePath)) {
                return true;
            }
        }
    }

    return false;
}
