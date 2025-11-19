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
        _text = text;
        ImGui::InputText(_title.c_str(), _text.AsChar(), nsString::MAX_SIZE - 1);
        if (!ImGui::IsItemActive()) {
            if (text != _text.AsChar()) {
                Log::Info("changed: %s", _text.AsChar());
                const TString str = _text.AsChar();
                nsUndoService::Shared()->Push(new nsUndoVarChange(text, str));
            }
        }
    }

private:
    std::string _title;
    nsString _text;
};
