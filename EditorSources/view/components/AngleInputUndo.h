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
        float value = nsMath::ToDeg(var);
        if (ImGui::InputFloat(_title.c_str(), &value)) {
            nsUndoService::Shared()->Push(new nsUndoVarChange(var, nsMath::ToRad(value)));
        }
    }

private:
    std::string _title;
};
