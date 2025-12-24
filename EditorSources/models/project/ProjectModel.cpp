//
// Created by Roman on 11/18/2025.
//

#include "ProjectModel.h"

#include "Engine/display/factory/VisualFactory2d.h"
#include "nsLib/log.h"

nsProjectModel::nsProjectModel() {
    _models.push_back(&scenes);
    _models.push_back(&particles);

    //project state should be loaded after all the models
    _models.push_back(&user);
}

bool nsProjectModel::Load(const nsFilePath &projectFolder) {
    Reset();
    Log::Info("Loading project from folder: %s", projectFolder.AsChar());
    nsVisualFactory2d::Shared()->assetsPath = projectFolder;

    for (const auto model: _models) {
        if (!model->Load(projectFolder)) {
            Reset();
            return false;
        }
    }

    for (const auto model: _models) {
        model->Validate(this);
    }

    return true;
}

bool nsProjectModel::Save(const nsFilePath &projectFolder) {
    Log::Info("Saving project to folder: %s", projectFolder.AsChar());

    bool res = true;
    for (const auto model: _models) {
        if (!model->Save(projectFolder)) {
            res = false;
        }
    }

    return res;
}

void nsProjectModel::Reset() {
    for (const auto model: _models) {
        model->Reset();
    }
}
