// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file EditorEventBus.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "EditorCommand.h"
#include "nsLib/SubSystem.h"
#include "nsLib/events/EventDispatcher.h"
#include <memory>
#include <utility>
#include <vector>

struct nsEditorEventName final {
    enum {
        FIT_SCENE_TO_VIEW = 0,
        CENTER_SCENE_100,
        NEW_PROJECT,
    };
};

class nsEditorEventBus final : public nsEventDispatcher, public nsSubSystem<nsEditorEventBus> {
public:
    void RegisterCommand(int eventId, std::shared_ptr<nsIEditorCommand> command);

    template<typename TCommand, typename... TArgs>
    void RegisterCommand(int eventId, TArgs&&... args) {
        RegisterCommand(eventId, std::make_shared<TCommand>(std::forward<TArgs>(args)...));
    }

protected:
    void OnRelease() override;

private:
    std::vector<std::shared_ptr<nsIEditorCommand>> _commands;
};
