//
// Created by Roman on 10/22/2024.
//

#include "SVModel.h"
#include "nsLib/log.h"

nsSVModel::nsSVModel() : blastParticles(0) {

}

nsFilePath nsSVModel::GetProjectPath() const {
    return nsFilePath(settings.projectPath.GetValue().c_str());
}

bool nsSVModel::Load() {
    if (settings.Load("editor.ggml")) {
        const nsFilePath path = GetProjectPath();

        if (!project.Load(path)) {
            return false;
        }

        user.Load(path.ResolvePath("user.ggml"));
        return true;
    }

    return false;
}

bool nsSVModel::Save() {
    if (!settings.Save("editor.ggml")) {
        return false;
    }

    const nsFilePath path = GetProjectPath();
    if (!project.Save(path)) {
        return false;
    }

    user.Save(path.ResolvePath("user.ggml"));
    return true;
}

void nsSVModel::OnCreated() {
    Log::Info("sv model created");
}
