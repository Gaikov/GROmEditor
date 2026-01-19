//
// Created by Roman on 12/11/2025.
//

#include "VisualHolder.h"

bool nsVisualHolder::IsRoot(const nsVisualObject2d *node) {
    const auto parent = node->GetParent();
    return !parent || dynamic_cast<nsVisualHolder*>(parent);
}

nsVisualContainer2d * nsVisualHolder::GetRoot(nsVisualObject2d *node) {
    if (!node) {
        return nullptr;
    }
    if (IsRoot(node)) {
        return dynamic_cast<nsVisualContainer2d*>(node);
    }
    return GetRoot(node->GetParent());
}
