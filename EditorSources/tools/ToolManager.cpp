// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ToolManager.cpp
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#include "ToolManager.h"

bool nsToolManager::OnPointerDown(nsVisualObject2d *scene, float x, float y, int pointerId) {
    if (!CanEdit()) return false;
    return _activeTool->OnPointerDown(scene, x, y, pointerId);
}

bool nsToolManager::OnPointerMove(nsVisualObject2d *scene, float x, float y, int pointerId) {
    if (!CanEdit()) return false;
    return _activeTool->OnPointerMove(scene, x, y, pointerId);
}

bool nsToolManager::OnPointerUp(nsVisualObject2d *scene, float x, float y, int pointerId) {
    if (!CanEdit()) return false;
    return _activeTool->OnPointerUp(scene, x, y, pointerId);
}

bool nsToolManager::OnKeyDown(int key, bool repeated, int mods) {
    if (!CanEdit()) return false;
    return _activeTool->OnKeyDown(key, repeated, mods);
}

bool nsToolManager::OnKeyUp(int key, int mods) {
    if (!CanEdit()) return false;
    return _activeTool->OnKeyUp(key, mods);
}

void nsToolManager::DrawOverlay() {
    if (!CanEdit()) return;
    if (_activeTool) {
        _activeTool->DrawOverlay();
    }
}
