//
// Created by Roman on 11/13/2025.
//

#include "ViewsRoot.h"

void nsViewsRoot::RemoveView(const nsBaseView *view) {
    const auto it = std::find_if(_views.begin(), _views.end(), [view](const auto& v) { return v.get() == view; });
    if (it != _views.end()) {
        _views.erase(it);
    }
}

void nsViewsRoot::Draw() const {
    for (const auto& view : _views) {
        view->Draw();
    }
}
