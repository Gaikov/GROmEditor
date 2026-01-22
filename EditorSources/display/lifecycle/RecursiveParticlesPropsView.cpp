//
// Created by Roman on 1/22/2026.
//

#include "RecursiveParticlesPropsView.h"
#include "Engine/display/container/VisualContainer2d.h"
#include "imgui/imgui.h"

nsRecursiveParticlesPropsView::nsRecursiveParticlesPropsView()
    : nsBaseVisualPropsView("Particles Playground") {
    auto &user = _model->project.user;
    user.selectedObject.AddHandler(nsPropChangedName::CHANGED, [&](const nsBaseEvent *) {
        _particles.clear();
        nsVisualObject2d *selected = user.selectedObject;
        if (const auto container = dynamic_cast<nsVisualContainer2d *>(selected)) {
            container->GetChildrenRecursive(_particles);
        } else if (const auto parts = dynamic_cast<nsVisualParticles *>(selected)) {
            _particles.push_back(parts);
        }
    });
}

bool nsRecursiveParticlesPropsView::IsSupport(nsVisualObject2d *target) {
    return _particles.size();
}

void nsRecursiveParticlesPropsView::Draw(nsVisualObject2d *target) {
    if (ImGui::Button("Blast All")) {
        for (const auto p: _particles) {
            auto &ps = p->GetSystem();
            if (!ps.spawnEnabled) {
                ps.Emit();
            }
        }
    }
}
