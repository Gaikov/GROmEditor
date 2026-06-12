// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ToolManager.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "ITool.h"
#include <memory>

class nsSceneView;

class nsToolManager {
public:
    nsToolManager() = default;

    bool CanEdit() const {
        return _activeTool && _activeTool->CanEdit();
    }

    template<typename T, typename... Args>
    T *ActivateTool(Args&&...args) {
        _activeTool = std::make_unique<T>(std::forward<Args>(args)...);
        return static_cast<T *>(_activeTool.get());
    }

    void DeactivateTool() {
        _activeTool.reset();
    }

    ITool *GetActiveTool() const {
        return _activeTool.get();
    }

    bool HasActiveTool() const {
        return _activeTool != nullptr;
    }

    bool OnPointerDown(nsSceneView *view, float x, float y, int pointerId);
    bool OnPointerMove(nsSceneView *view, float x, float y, int pointerId);
    bool OnPointerUp(nsSceneView *view, float x, float y, int pointerId);
    bool OnKeyDown(int key, bool repeated, int mods);
    bool OnKeyUp(int key, int mods);

    void DrawOverlay();

private:
    std::unique_ptr<ITool> _activeTool;
};
