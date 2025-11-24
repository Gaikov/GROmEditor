//
// Created by Roman on 11/19/2025.
//

#include "ProjectStateModel.h"

nsProjectStateModel::nsProjectStateModel()
    : nsSerializableFile("user"),
      currentScene("current_scene", ""),
      xFlip("x_flip", false),
      yFlip("y_flip", false),
      emitParticles("emit_particles", true),
      backColor("back_color", nsColor::gray),
      testView("test_view", false) {
    AddItem(&currentScene);
    AddItem(&xFlip);
    AddItem(&yFlip);
    AddItem(&emitParticles);
    AddItem(&backColor);
    AddItem(&testView);
}
