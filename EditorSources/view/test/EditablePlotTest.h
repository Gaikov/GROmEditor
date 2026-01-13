//
// Created by Roman on 1/13/2026.
//

#ifndef NSBGAMEPROJECT_EDITABLEPLOTTEST_H
#define NSBGAMEPROJECT_EDITABLEPLOTTEST_H
#include <vector>

#include "nsLib/Vec2.h"


struct EditablePlot {
    std::vector<nsVec2> points;
    std::vector<float> xs, ys;
    int selected_point = -1;
    float point_radius = 6.0f;

    EditablePlot();

    void Draw();
};


#endif //NSBGAMEPROJECT_EDITABLEPLOTTEST_H