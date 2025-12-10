// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file EnumInputUndo.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include <string>
#include <vector>

#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVarChange.h"
#include "imgui/imgui.h"

template<typename TEnum>
class nsEnumInputUndo {
    struct EnumValue {
        std::string name;
        TEnum value;
    };

public:
    nsEnumInputUndo(const char *title) : _title(title) {
    }

    void AddVariant(const char *name, TEnum value) {
        _values.emplace_back(EnumValue{name, value});
    }

    void Draw(TEnum &value) {
        int selectedIndex = 0;
        for (int i = 0; i < _values.size(); i++) {
            auto &v = _values[i];
            if (v.value == value) {
                selectedIndex = i;
                break;
            }
        }

        if (ImGui::BeginCombo(_title.c_str(), _values[selectedIndex].name.c_str())) {
            for (int i = 0; i < _values.size(); i++) {
                bool isSelected = (selectedIndex == i);
                if (ImGui::Selectable(_values[i].name.c_str(), isSelected)) {
                    selectedIndex = i;
                    nsUndoService::Shared()->Push(new nsUndoVarChange(value, _values[selectedIndex].value));
                }

                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

private:
    std::string _title;
    std::vector<EnumValue> _values;
};
