// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file VisualRefBuilder.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "Engine/display/VisualType.h"
#include "Engine/display/factory/VisualBuilder2d.h"

class nsVisualRefBuilder : public nsVisualBuilder2d {
public:
    static constexpr auto NAME = nsVisualType::LAYOUT_REF;

    nsVisualObject2d * CreateDefault() override;
    bool Parse(script_state_t *ss, nsVisualObject2d *object, nsVisualCreationContext2d *context) override;

protected:
    bool SerializeProps(nsScriptSaver &saver, nsVisualObject2d *o, nsVisualCreationContext2d *context) override;
};
