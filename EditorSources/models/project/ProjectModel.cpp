//
// Created by Roman on 11/18/2025.
//

#include "ProjectModel.h"

nsProjectModel::nsProjectModel()
    : nsSerializableFile("project"),
      projectPath(""),
      currentScene("current_scene", "") {

    AddItem(&currentScene);
}
