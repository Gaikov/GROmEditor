//
// Created by Roman on 11/18/2025.
//

#include "ProjectModel.h"

#include "nsLib/log.h"

bool nsProjectModel::Load(const nsFilePath &projectFolder) {
    Log::Info("Loading project from folder: %s", projectFolder.AsChar());
    if (!scenes.Load(projectFolder)) {
        return false;
    }

    return true;
}

bool nsProjectModel::Save(const nsFilePath &projectFolder) {
    Log::Info("Saving project to folder: %s", projectFolder.AsChar());
    scenes.Save(projectFolder);

    return true;
}

