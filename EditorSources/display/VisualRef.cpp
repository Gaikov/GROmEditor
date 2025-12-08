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
        _ref->DrawNode(context);
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
    if (_ref) {
        _ref->OnAddedToStage();
    }
    UpdateRef();
}

void nsVisualRef::OnRemovedFromStage() {
    nsVisualObject2d::OnRemovedFromStage();
    if (_ref) {
        _ref->OnRemovedFromStage();
    }
}

void nsVisualRef::UpdateRef() {
    if (_ref) {
        if (IsOnStage()) {
            _ref->OnRemovedFromStage();
        }
        origin.RemoveChild(&_ref->origin);
        _ref->Destroy();
    }
    _ref = _context->Create(source->c_str());

    if (_ref) {
        origin.AddChild(&_ref->origin);
        if (IsOnStage()) {
            _ref->OnAddedToStage();
        }

        _itemPos = _ref->origin.pos;
        _itemScale = _ref->origin.scale;
        _itemAngle = _ref->origin.angle;
        _itemVisible = _ref->visible;

        _ref->origin.Reset();
        _ref->visible = true;
    }

    Log::Debug("Visual ref updated: %s", source->c_str());
}