// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file SceneUtils.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "Engine/display/VisualObject2d.h"
#include "nsLib/color.h"

class nsSceneUtils {
public:
    static void DrawBounds(nsVisualObject2d *obj, const nsColor &color = nsColor::green);
    static void DrawOrigin(nsVisualObject2d *obj, const nsColor &color = nsColor::magenta);
};
