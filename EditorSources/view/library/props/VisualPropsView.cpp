//
// Created by Roman on 11/19/2025.
//

#include "VisualPropsView.h"

void nsVisualPropsView::Draw(nsVisualObject2d *target) {
    _idInput.Draw(target->id);
    _visibleInput.Draw(target->visible);
    _posInput.Draw(target->origin.pos);
    _scaleInput.Draw(target->origin.scale);
    _angleInput.Draw(target->origin.angle);
}
