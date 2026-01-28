// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file CustomPropView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "Engine/display/property/VisualCustomProp.h"
#include "models/project/CustomVisualsModel.h"
#include "view/components/EnumInputUndo.h"

class nsCustomPropView {
public:
    using sp_t = std::shared_ptr<nsCustomPropView>;

    virtual ~nsCustomPropView() = default;
    virtual void DrawProperty(const char *name, nsVisualCustomProp *prop, nsCustomVisualPropertyMeta *meta) = 0;
};

class nsCustomBoolView final : public nsCustomPropView {
public:
    void DrawProperty(const char *name, nsVisualCustomProp *prop, nsCustomVisualPropertyMeta *meta) override;
};

class nsCustomEnumView final : public nsCustomPropView {
public:
    void DrawProperty(const char *name, nsVisualCustomProp *prop, nsCustomVisualPropertyMeta *meta) override;
private:
    nsVisualCustomProp *_lastProp = nullptr;
    nsEnumInputUndo<std::string, nsStringVar> _inputEnum = "";
};

