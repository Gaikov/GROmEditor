// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file FloatInputUndo.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include <string>

#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVarChange.h"
#include "imgui/imgui.h"

template<typename TFloat>
class nsFloatInputUndo {
public:
    float step = 0.01f;
    float fastStep = 1;

    nsFloatInputUndo(const char *title) : _title(title) {
    };

    void Draw(TFloat &var) {
        float value = var;
        if (ImGui::InputFloat(_title.c_str(), &value, step, fastStep)) {
            nsUndoService::Shared()->Push(new nsUndoVarChange(var, value));
        }
    }

private:
    std::string _title;
};
