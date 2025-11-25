// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file BoolInputUndo.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include <string>

#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVarChange.h"
#include "imgui/imgui.h"

template<typename TVar>
class nsBoolInputUndo {
public:
    nsBoolInputUndo(const char *title) : _title(title) {
    }

    void Draw(TVar &var) {
        bool value = var;
        if (ImGui::Checkbox(_title.c_str(), &value)) {
            nsUndoService::Shared()->Push(new nsUndoVarChange(var, value));
        }
    }

private:
    std::string _title;
};
