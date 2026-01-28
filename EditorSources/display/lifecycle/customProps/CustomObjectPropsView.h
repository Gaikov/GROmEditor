// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file CustomObjectPropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "CustomPropView.h"
#include "view/library/props/BaseVisualPropsView.h"
#include "Engine/display/property/VisualCustomPropFactory.h"

class nsCustomObjectPropsView : public nsBaseVisualPropsView {
public:
    nsCustomObjectPropsView();
    bool IsSupport(nsVisualObject2d *target) override;

protected:
    nsVisualCustomPropFactory _factory;
    std::map<nsVisualCustomProp::Type, nsCustomPropView::sp_t> _views;

    void Draw(nsVisualObject2d *target) override;
    void DrawProperty(const char *name, nsVisualCustomProp *prop, nsCustomVisualPropertyMeta *meta);
};
