//
// Created by Roman on 11/19/2025.
//

#include "ProjectStateModel.h"

#include "ProjectModel.h"

nsProjectStateModel::nsProjectStateModel() {
    AddItem("current_scene", &currentScene);
    AddItem("x_flip", &xFlip);
    AddItem("y_flip", &yFlip);
    AddItem("zoom", &zoom);
    AddItem("back_color", &backColor);
    AddItem("scene_x", &sceneX);
    AddItem("scene_y", &sceneY);
    AddItem("test_view", &testView);
    AddItem("back_cell_size", &backCellSize);
}

void nsProjectStateModel::Reset() {
    ResetDefault();
}

bool nsProjectStateModel::Load(const nsFilePath &folder) {
    return nsSerializableFile::Load(folder.ResolvePath("user.ggml"));
}

bool nsProjectStateModel::Save(const nsFilePath &folder) {
    return nsSerializableFile::Save(folder.ResolvePath("user.ggml"));
}

void nsProjectStateModel::Validate(nsProjectModel *model) {
    nsProjectSubModel::Validate(model);

    if (!model->scenes.Get(currentScene)) {
        currentScene = "";
    }
}
