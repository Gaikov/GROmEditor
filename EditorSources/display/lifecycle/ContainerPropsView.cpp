//
// Created by Roman on 12/11/2025.
//

#include "ContainerPropsView.h"

#include "Engine/RenManager.h"
#include "Engine/display/container/VisualContainer2d.h"
#include "Engine/display/particles/VisualParticles.h"
#include "Engine/display/sprite/Sprite.h"
#include "Engine/display/text/TextLabel.h"

bool nsContainerPropsView::IsSupport(nsVisualObject2d *target) {
    return dynamic_cast<nsVisualContainer2d*>(target);
}

bool nsContainerPropsView::DrawContextMenu(nsVisualObject2d *target, bool hasPrevItems) {
    const auto c = dynamic_cast<nsVisualContainer2d*>(target);

    if (ImGui::BeginMenu("Create...")) {
        if (const auto sprite = MenuItemCreate<nsSprite>("Sprite", c)) {
            sprite->renState = GetDefaultRenState();
        }
        if (const auto label = MenuItemCreate<nsTextLabel>("Label", c)) {
            label->renState = GetDefaultRenState();
        }
        MenuItemCreate<nsVisualParticles>("Particles", c);

        ImGui::EndMenu();
    }
    return true;
}

void nsContainerPropsView::Draw(nsVisualObject2d *target) {
    const auto c = dynamic_cast<nsVisualContainer2d*>(target);
    _interactiveInput.Draw(c->interactiveChildren);
}

IRenState * nsContainerPropsView::GetDefaultRenState() const {
    const auto path = _model->GetProjectPath().ResolvePath("default/rs/gui.ggrs");
    return nsRenDevice::Shared()->Device()->StateLoad(path);
}
