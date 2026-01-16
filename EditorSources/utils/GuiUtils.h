// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file GuiUtils.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "imgui/imgui.h"
#include "nsLib/color.h"

class nsGuiUtils {
public:
    static ImU32 ColorU32(const nsColor &color);
};
