//
// Created by Roman on 6/12/2026.
//

#include "AnchorPropsView.h"
#include "Engine/display/helpers/VisualAnchor.h"

bool nsAnchorPropsView::IsSupport(nsVisualObject2d *target) {
    return dynamic_cast<nsVisualAnchor*>(target) != nullptr;
}

void nsAnchorPropsView::Draw(nsVisualObject2d *target) {
    const auto anchor = dynamic_cast<nsVisualAnchor*>(target);

    _sizeInput.Draw(anchor->size);
    _colorInput.Draw(anchor->color);
    _renStateInput.Draw(anchor->renState);
}
