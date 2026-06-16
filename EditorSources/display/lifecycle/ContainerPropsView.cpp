//
// Created by Roman on 12/11/2025.
//

#include "ContainerPropsView.h"

#include "Engine/RenManager.h"
#include "Engine/display/container/VisualContainer2d.h"
#include "Engine/display/graphics/VisualPolygon.h"
#include "Engine/display/helpers/VisualAnchor.h"
#include "Engine/display/particles/VisualParticles.h"
#include "Engine/display/sprite/Sprite.h"
#include "Engine/display/text/TextLabel.h"
#include "display/VisualRef.h"

bool nsContainerPropsView::IsSupport(nsVisualObject2d *target) {
    return dynamic_cast<nsVisualContainer2d*>(target);
}

bool nsContainerPropsView::DrawContextMenu(nsVisualObject2d *target, bool hasPrevItems) {
    const auto c = dynamic_cast<nsVisualContainer2d*>(target);

    if (ImGui::BeginMenu("Create...")) {
        MenuItemCreate<nsVisualContainer2d>(nsVisualType::CONTAINER, c);

        if (const auto sprite = MenuItemCreate<nsSprite>(nsVisualType::SPRITE, c)) {
            sprite->renState = GetDefaultRenState();
        }
        if (const auto label = MenuItemCreate<nsTextLabel>(nsVisualType::LABEL, c)) {
            label->renState = GetDefaultRenState();
        }
        MenuItemCreate<nsVisualRef>(nsVisualType::LAYOUT_REF, c);

        MenuItemCreate<nsVisualParticles>(nsVisualType::PARTICLES, c);
        MenuItemCreate<nsVisualAnchor>(nsVisualType::ANCHOR, c);
        if (const auto polygon = MenuItemCreate<nsVisualPolygon>(nsVisualType::POLYGON, c)) {
            polygon->points = {{0, 0}, {100, 0}, {100, 100}, {0, 100}};
        }

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
