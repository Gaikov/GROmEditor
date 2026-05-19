// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file NewProjectCommand.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "EditorCommand.h"

class nsNewProjectCommand final : public nsIEditorCommand {
public:
    void Execute(const nsBaseEvent *event) override;
};
