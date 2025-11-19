// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file VisualPropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include <memory>
#include "Engine/display/VisualObject2d.h"

class nsBaseVisualPropsView {
public:
    typedef std::shared_ptr<nsBaseVisualPropsView> sp_t;

    virtual ~nsBaseVisualPropsView() = default;
    virtual void Draw(nsVisualObject2d *target) = 0;
};
