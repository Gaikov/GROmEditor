//
// Created by Roman on 12/11/2025.
//

#include "VisualsLifecycle.h"

#include "ContainerPropsView.h"
#include "view/library/props/LayoutRefPropsView.h"
#include "SpritePropsView.h"
#include "view/library/props/TextLabelPropsView.h"
#include "view/library/props/VisualPropsView.h"

nsVisualsLifecycle::nsVisualsLifecycle() {
    _policies.emplace_back(new nsVisualPropsView());
    _policies.emplace_back(new nsContainerPropsView());
    _policies.emplace_back(new nsLayoutRefPropsView());
    _policies.emplace_back(new nsSpritePropsView());
    _policies.emplace_back(new nsTextLabelPropsView());
}

void nsVisualsLifecycle::DrawProps(nsVisualObject2d *target) const {
    for (const auto &propsView: _policies) {
        if (propsView->IsSupport(target)) {
            propsView->DrawPanel(target);
        }
    }
}

void nsVisualsLifecycle::DrawContextMenu(nsVisualObject2d *target) const {
    bool hasPrevItems = false;
    for (int i = _policies.size() - 1; i >= 0; --i) {
        auto &p = _policies[i];
        if (p->IsSupport(target)) {
            if (p->DrawContextMenu(target, hasPrevItems)) {
                hasPrevItems = true;
            }
        }
    }
}