//
// Created by Roman on 12/11/2025.
//

#include "VisualHolder.h"

bool nsVisualHolder::isRoot(const nsVisualObject2d *node) {
    const auto parent = node->GetParent();
    return !parent || dynamic_cast<nsVisualHolder*>(parent);
}
