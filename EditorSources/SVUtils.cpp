//
// Created by Roman on 10/21/2024.
//

#include "SVUtils.h"

#include "Engine/display/container/VisualContainer2d.h"
#include "nsLib/log.h"

nsBaseButton * nsSVUtils::GetButton(nsVisualContainer2d *container, const char *buttonId) {
    auto child = container->GetChildByIdRecursive(buttonId);
    if (!child) {
        Log::Warning("child not found by id: %s",  buttonId);
        return nullptr;
    }

    auto button = dynamic_cast<nsBaseButton*>(child);
    if (!button) {
        Log::Warning("child is not a button: %s", buttonId);
        return nullptr;
    }

    return button;
}

void nsSVUtils::OnButtonClick(nsVisualContainer2d *container, const char *buttonId, const nsBaseButton::handler_t &handler) {
    GetButton(container, buttonId)->SetClickHandler(handler);
}
