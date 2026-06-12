// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ToolManager.cpp
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#include "ToolManager.h"
#include "SceneView.h"

bool nsToolManager::OnPointerDown(nsSceneView *view, float x, float y, int pointerId) {
    if (!CanEdit()) return false;
    return _activeTool->OnPointerDown(view, x, y, pointerId);
}

bool nsToolManager::OnPointerMove(nsSceneView *view, float x, float y, int pointerId) {
    if (!CanEdit()) return false;
    return _activeTool->OnPointerMove(view, x, y, pointerId);
}

bool nsToolManager::OnPointerUp(nsSceneView *view, float x, float y, int pointerId) {
    if (!CanEdit()) return false;
    return _activeTool->OnPointerUp(view, x, y, pointerId);
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
    if (_activeTool) {
        _activeTool->DrawOverlay();
    }
}
