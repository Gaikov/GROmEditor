// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file IntInputUndo.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVarChange.h"
#include "imgui/imgui.h"

template<typename TInt>
class nsIntInputUndo {
public:
    float step = 0.01f;
    float fastStep = 1;

    nsIntInputUndo(const char *title) : _title(title) {
    };

    void Draw(TInt &var) {
        int value = var;
        if (ImGui::InputInt(_title.c_str(), &value, step, fastStep)) {
            nsUndoService::Shared()->Push(new nsUndoVarChange(var, value));
        }
    }

private:
    std::string _title;
};
