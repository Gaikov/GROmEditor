// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ITool.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

class nsVisualObject2d;

class ITool {
public:
    virtual ~ITool() = default;

    virtual bool CanEdit() const = 0;
    virtual bool OnPointerDown(nsVisualObject2d *scene, float x, float y, int pointerId) = 0;
    virtual bool OnPointerMove(nsVisualObject2d *scene, float x, float y, int pointerId) = 0;
    virtual bool OnPointerUp(nsVisualObject2d *scene, float x, float y, int pointerId) = 0;

    virtual bool OnKeyDown(int key, bool repeated, int mods) { return false; }
    virtual bool OnKeyUp(int key, int mods) { return false; }

    virtual void DrawOverlay() {}
    virtual const char *GetName() const = 0;
};
