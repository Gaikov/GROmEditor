// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file UndoTextInput.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVarChange.h"
#include "imgui/imgui.h"
#include "nsLib/log.h"
#include "nsLib/StrTools.h"

template<typename TString>
class nsTextInputUndo {
public:
    nsTextInputUndo(const char *title) : _title(title) {
    }

    void Draw(TString &text) {
        DrawField(_title.c_str(), text);
    }

    static void DrawField(const char *title, TString &text) {
        nsString current;
        current = text;
        ImGui::InputText(title, current.AsChar(), nsString::MAX_SIZE - 1);
        if (!ImGui::IsItemActive()) {
            if (text != current.AsChar()) {
                Log::Info("changed: %s", current.AsChar());
                std::string str = current.AsChar();
                nsUndoService::Shared()->Push(new nsUndoVarChange(text, str));
            }
        }
    }

private:
    std::string _title;
};
