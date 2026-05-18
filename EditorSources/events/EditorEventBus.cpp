//
// Created by Roman on 5/18/2026.
//

#include "EditorEventBus.h"

void nsEditorEventBus::RegisterCommand(const int eventId, std::shared_ptr<nsIEditorCommand> command) {
    const auto cmd = std::move(command);
    _commands.push_back(cmd);
    AddHandler(eventId, [cmd](const nsBaseEvent *event) {
        cmd->Execute(event);
    });
}

void nsEditorEventBus::OnRelease() {
    _commands.clear();
}
