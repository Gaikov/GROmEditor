// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file MenuBar.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "MenuItem.h"

class nsMenuBar final : public nsMenuItem {
public:
    explicit nsMenuBar()
        : nsMenuItem("") {
    }

    void Draw() override;
};
