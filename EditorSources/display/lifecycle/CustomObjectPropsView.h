// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file CustomObjectPropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "view/library/props/BaseVisualPropsView.h"
#include "Engine/display/property/VisualCustomPropFactory.h"

class nsCustomObjectPropsView : public nsBaseVisualPropsView {
public:
    explicit nsCustomObjectPropsView()
        : nsBaseVisualPropsView("Custom Object") {
    }
    bool IsSupport(nsVisualObject2d *target) override;

protected:
    nsVisualCustomPropFactory _factory;

    void Draw(nsVisualObject2d *target) override;
};
