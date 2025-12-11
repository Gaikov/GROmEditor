//
// Created by Roman on 12/11/2025.
//

#include "VisualsLifecycle.h"

#include "view/library/props/LayoutRefPropsView.h"
#include "view/library/props/SpritePropsView.h"
#include "view/library/props/TextLabelPropsView.h"
#include "view/library/props/VisualPropsView.h"

nsVisualsLifecycle::nsVisualsLifecycle() {
    _policies.emplace_back(new nsVisualPropsView());
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
