// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file EditorCommand.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "nsLib/events/BaseEvent.h"

class nsIEditorCommand {
public:
    virtual ~nsIEditorCommand() = default;

    virtual void Execute(const nsBaseEvent *event) = 0;
};
