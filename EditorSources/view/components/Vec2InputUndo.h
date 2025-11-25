// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file Vec2InputUndo.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include <string>

#include "Core/undo/UndoService.h"
#include "imgui/imgui.h"
#include "nsLib/log.h"
#include "nsLib/Vec2.h"

template<typename TVec2>
class nsVec2InputUndo {
public:
    nsVec2InputUndo(const char *title) : _title(title) {
    }

    void Draw(TVec2 &var) {
        nsVec2 value;
        const nsVec2 oldValue = value = var;

        if (ImGui::InputFloat2(_title.c_str(), value)) {
        //if (!ImGui::IsItemActive()) {
            if (oldValue != value) {
                Log::Info("changed: %s", value.ToStr());
                nsUndoService::Shared()->Push(new nsUndoVarChange(var, value));
            }
        }
    }

private:
    std::string _title;
};
