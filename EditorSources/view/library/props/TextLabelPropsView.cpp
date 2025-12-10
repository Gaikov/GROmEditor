//
// Created by Roman on 12/10/2025.
//

#include "TextLabelPropsView.h"
#include "Engine/display/text/TextLabel.h"

bool nsTextLabelPropsView::IsSupport(nsVisualObject2d *target) {
    return dynamic_cast<nsTextLabel*>(target);
}

void nsTextLabelPropsView::Draw(nsVisualObject2d *target) {
    const auto label = dynamic_cast<nsTextLabel*>(target);

    _renStateSelect.Draw(label->renState);
    _fontSelect.Draw(label->font);
}
