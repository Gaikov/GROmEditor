//
// Created by Roman on 11/19/2025.
//

#include "ProjectStateModel.h"

nsProjectStateModel::nsProjectStateModel()
    : nsSerializableFile("user"),
      currentScene("current_scene", "") {
    AddItem(&currentScene);
}
