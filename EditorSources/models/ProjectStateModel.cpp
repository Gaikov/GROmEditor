//
// Created by Roman on 11/19/2025.
//

#include "ProjectStateModel.h"

nsProjectStateModel::nsProjectStateModel()
    : nsSerializableFile("user"),
      currentScene("current_scene", ""),
      xFlip("x_flip", false),
      yFlip("y_flip", false),
      zoom("zoom", 1.0f),
      backColor("back_color", nsColor::gray),
      sceneX("scene_x", 0), sceneY("scene_y", 0),
      testView("test_view", false) {

    AddItem(&currentScene);
    AddItem(&xFlip);
    AddItem(&yFlip);
    AddItem(&zoom);
    AddItem(&backColor);
    AddItem(&sceneX);
    AddItem(&sceneY);
    AddItem(&testView);
}
