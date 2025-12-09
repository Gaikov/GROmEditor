//
// Created by Roman on 11/18/2025.
//

#include "ProjectModel.h"

#include "Engine/display/factory/VisualFactory2d.h"
#include "nsLib/log.h"

bool nsProjectModel::Load(const nsFilePath &projectFolder) {
    Log::Info("Loading project from folder: %s", projectFolder.AsChar());
    nsVisualFactory2d::Shared()->assetsPath = projectFolder;
    if (!scenes.Load(projectFolder)) {
        return false;
    }

    return true;
}

bool nsProjectModel::Save(const nsFilePath &projectFolder) const {
    Log::Info("Saving project to folder: %s", projectFolder.AsChar());
    bool res = scenes.Save(projectFolder);

    return res;
}

