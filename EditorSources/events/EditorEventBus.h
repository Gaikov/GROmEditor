// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file EditorEventBus.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "nsLib/SubSystem.h"
#include "nsLib/events/EventDispatcher.h"

struct nsEditorEventName final {
    enum {
        FIT_SCENE_TO_VIEW = 0,
        CENTER_SCENE_100,
    };
};

class nsEditorEventBus final : public nsEventDispatcher, public nsSubSystem<nsEditorEventBus> {
};
