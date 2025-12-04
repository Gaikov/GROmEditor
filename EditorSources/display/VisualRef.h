// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file VisualLayoutRef.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "Engine/display/VisualObject2d.h"
#include "Engine/display/factory/VisualCreationContext.h"

class nsVisualRef : public nsVisualObject2d {
    friend class nsVisualRefBuilder;
public:
    nsVisualRef();

    nsProperty<std::string> source;

    const char * GetType() override;
    void GetLocalBounds(nsRect &bounds) override;
    void Loop() override;
    void DrawContent(const nsVisualContext2d &context) override;

    nsVisualObject2d* GetRef() const { return _ref; }
    void Destroy() override;

private:
    nsVisualObject2d *_ref = nullptr;
    nsVisualCreationContext2d *_context = nullptr;
};
