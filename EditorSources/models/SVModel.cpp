//
// Created by Roman on 10/22/2024.
//

#include "SVModel.h"
#include "nsLib/log.h"

nsSVModel::nsSVModel()
    : nsSerializableFile("user"),
      projectPath("project_path", "."),
      blastParticles(0) {

    AddItem(&projectPath);
}

nsFilePath nsSVModel::GetProjectPath() {
    return nsFilePath(projectPath.GetValue().c_str());
}

bool nsSVModel::Load(const char *fileName) {
    if (nsSerializableFile::Load(fileName)) {
        const nsFilePath path = GetProjectPath();
        user.Load(path.ResolvePath("user.ggml"));
        return true;
    }
    return false;
}

bool nsSVModel::Save(const char *fileName) {
    if (nsSerializableFile::Save(fileName)) {
        const nsFilePath path = GetProjectPath();
        user.Save(path.ResolvePath("user.ggml"));
        return true;
    }
    return false;
}

void nsSVModel::OnCreated() {
    Log::Info("sv model created");
}
