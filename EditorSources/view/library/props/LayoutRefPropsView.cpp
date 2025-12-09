//
// Created by Roman on 12/9/2025.
//

#include "LayoutRefPropsView.h"

#include "display/VisualRef.h"

bool nsLayoutRefPropsView::IsSupport(nsVisualObject2d *target) {
    return dynamic_cast<nsVisualRef*>(target);
}

void nsLayoutRefPropsView::Draw(nsVisualObject2d *target) {
    const auto ref = dynamic_cast<nsVisualRef*>(target);
    _layoutSelect.Draw(ref->source);
}
