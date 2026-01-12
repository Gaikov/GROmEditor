// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file AngleInputUndo.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVarChange.h"
#include "imgui/imgui.h"
#include "nsLib/MathTools.h"


template<typename TVar>
class nsAngleInputUndo {
public:
    nsAngleInputUndo(const char *title) : _title(title) {
    }

    void Draw(TVar &var) {
        DrawField(_title.c_str(), var);
    }

    static void DrawField(const char *title, TVar &var) {
        float value = nsMath::ToDeg(var);
        if (ImGui::InputFloat(title, &value, 1.0f, 10.0f)) {
            nsUndoService::Shared()->Push(new nsUndoVarChange(var, nsMath::ToRad(value)));
        }
    }

private:
    std::string _title;
};
