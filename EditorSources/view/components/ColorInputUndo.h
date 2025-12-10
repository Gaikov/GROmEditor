// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ColorInputUndo.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVarChange.h"
#include "imgui/imgui.h"
#include "nsLib/color.h"
#include "nsLib/log.h"

template<typename TColor>
class nsColorInputUndo {
public:
    nsColorInputUndo(const char *title) : _title(title) {
    }

    void Draw(TColor &var) {
        ImGui::ColorEdit4(_title.c_str(), _color);
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            Log::Info("color changed");
            nsUndoService::Shared()->Push(new nsUndoVarChange(var, _color));
        }
        if (!ImGui::IsItemActive()) {
            _color = var;
        }
    }

private:
    std::string _title;
    nsColor _color;
    bool _active = false;
};
