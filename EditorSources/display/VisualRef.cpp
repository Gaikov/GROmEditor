//
// Created by Roman on 12/4/2025.
//

#include "VisualRef.h"
#include "Engine/display/VisualType.h"
#include "nsLib/log.h"

nsVisualRef::nsVisualRef() : source("") {
    source.AddHandler(nsPropChangedName::CHANGED, [this](const nsBaseEvent *) {
        UpdateRef();
    });
}

const char *nsVisualRef::GetType() {
    return nsVisualType::LAYOUT_REF;
}

void nsVisualRef::GetLocalBounds(nsRect &bounds) {
    if (_ref) {
        _ref->GetLocalBounds(bounds);
    } else {
        constexpr float size = 10;
        bounds = {-size / 2, -size / 2, size, size};
    }
}

void nsVisualRef::Loop() {
    if (_ref) {
        _ref->Loop();
    }
}

void nsVisualRef::DrawContent(const nsVisualContext2d &context) {
    if (_ref) {
        _ref->DrawContent(context);
    }
}

void nsVisualRef::Destroy() {
    if (_ref) {
        _ref->Destroy();
    }
    nsVisualObject2d::Destroy();
}

void nsVisualRef::OnAddedToStage() {
    nsVisualObject2d::OnAddedToStage();
    UpdateRef();
}

void nsVisualRef::UpdateRef() {
    if (_ref) {
        _ref->Destroy();
    }
    _ref = _context->Create(source->c_str());
    Log::Debug("Visual ref updated: %s", source->c_str());
}
