//
// Created by Roman on 10/22/2024.
//

#include "AppModel.h"
#include "nsLib/log.h"

nsAppModel::nsAppModel() : blastParticles(0) {

}

nsFilePath nsAppModel::GetProjectPath() const {
    return nsFilePath(settings.projectPath.GetValue().c_str());
}

bool nsAppModel::Load() {
    if (settings.Load("editor.ggml")) {
        const nsFilePath path = GetProjectPath();

        if (!project.Load(path)) {
            return false;
        }

        return true;
    }

    return false;
}

bool nsAppModel::Save() {
    if (!settings.Save("editor.ggml")) {
        return false;
    }

    return true;
}

void nsAppModel::OnCreated() {
    Log::Info("sv model created");
}
