// Copyright (c) 2003-2024, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file SVUtils.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "Engine/display/button/BaseButton.h"

struct nsSVUtils final {
    static nsBaseButton* GetButton(nsVisualContainer2d *container, const char *buttonId);
};
