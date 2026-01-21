//
// Created by Roman on 1/21/2026.
//

#include "CompositeView.h"

void nsCompositeView::Draw() {
    for (const auto& view : _views) {
        if (view->visible) {
            view->Draw();
        }
    }
}

void nsCompositeView::PostDraw() {
    nsBaseView::PostDraw();

    for (const auto& view : _views) {
        if (view->visible) {
            view->PostDraw();
        }
    }
}
