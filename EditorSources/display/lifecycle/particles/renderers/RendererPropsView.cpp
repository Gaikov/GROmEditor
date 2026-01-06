//
// Created by Roman on 1/6/2026.
//

#include "RendererPropsView.h"

nsRendererPropsRegistry::nsRendererPropsRegistry() {
}

void nsRendererPropsRegistry::DrawProps(nsParticlesRenderer *r) const {
    for (const auto &prop : m_props) {
        if (prop->IsSupported(r)) {
            prop->Draw(r);
        }
    }
}
