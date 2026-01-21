// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ViewsRoot.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "CompositeView.h"
#include "nsLib/SubSystem.h"

class nsViewsRoot final : public nsCompositeView, public nsSubSystem<nsViewsRoot> {
public:
    void Draw() override;
protected:
    bool OnInit() override;
};

