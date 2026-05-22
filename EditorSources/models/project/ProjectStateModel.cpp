//
// Created by Roman on 11/19/2025.
//

#include "ProjectStateModel.h"

#include "ProjectModel.h"
#include "Engine/display/container/VisualContainer2d.h"

nsSceneState::nsSceneState() {
    AddItem("scene", &scene);
    AddItem("zoom", &zoom);
    AddItem("x", &x);
    AddItem("y", &y);
}

nsProjectStateModel::nsProjectStateModel() {
    AddItem("current_scene", &currentScene);
    AddItem("scene_state", &sceneStates);
    AddItem("x_flip", &xFlip);
    AddItem("y_flip", &yFlip);
    AddItem("back_color", &backColor);
    AddItem("test_view", &testView);
    AddItem("back_cell_size", &backCellSize);
    AddItem("show_project_settings", &showProjectSettings);
}

bool nsProjectStateModel::SaveUserState(const nsFilePath &folder) {
    return Save(folder);
}

void nsProjectStateModel::Reset() {
    _projectModel = nullptr;
    selectedObject = nullptr;
    ResetDefault();
}

void nsProjectStateModel::SetBaseFolder(const nsFilePath &folder) {
    _projectFolder = folder;
    currentScene.SetBaseFolder(folder);
    for (int i = 0; i < sceneStates.Size(); ++i) {
        if (const auto state = sceneStates.GetItem(i)) {
            state->scene.SetBaseFolder(folder);
        }
    }
}

nsSceneState *nsProjectStateModel::FindSceneState(const std::string &scenePath) {
    for (int i = 0; i < sceneStates.Size(); ++i) {
        if (auto state = sceneStates.GetItem(i); state && state->scene == scenePath) {
            return state;
        }
    }
    return nullptr;
}

nsSceneState *nsProjectStateModel::GetSceneState() {
    const auto &path = currentScene.GetValue();
    if (path.empty() || !_projectModel || !_projectModel->scenes.Get(path)) {
        return nullptr;
    }

    if (auto state = FindSceneState(path)) {
        return state;
    }

    const auto item = std::make_shared<nsSceneState>();
    item->scene.SetBaseFolder(_projectFolder);
    item->scene = path;
    sceneStates.Add(item);
    return dynamic_cast<nsSceneState *>(item.get());
}

bool nsProjectStateModel::Load(const nsFilePath &folder) {
    SetBaseFolder(folder);
    nsSerializableFile::Load(folder.ResolvePath("user.ggml"));
    SetBaseFolder(folder);
    return true;
}

bool nsProjectStateModel::Save(const nsFilePath &folder) {
    SetBaseFolder(folder);
    return nsSerializableFile::Save(folder.ResolvePath("user.ggml"));
}

void nsProjectStateModel::Validate(nsProjectModel *model) {
    nsProjectSubModel::Validate(model);
    _projectModel = model;

    if (!model->scenes.Get(currentScene)) {
        currentScene = "";
    }

    for (int i = sceneStates.Size() - 1; i >= 0; --i) {
        if (const auto state = sceneStates.GetItem(i); !state || !model->scenes.Get(state->scene)) {
            sceneStates.RemoveAt(i);
        }
    }
}
