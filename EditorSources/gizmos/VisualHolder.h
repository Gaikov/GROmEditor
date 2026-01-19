// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file VisualHolder.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "Engine/display/container/VisualContainer2d.h"

class nsVisualHolder : public nsVisualContainer2d {
public:
    ~nsVisualHolder() override = default;

    static bool IsRoot(const nsVisualObject2d *node);
    static nsVisualContainer2d *GetRoot(nsVisualObject2d *node);
};
