// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file RenStateSelectUndo.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "BaseAssetSelect.h"
#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVarChange.h"
#include "Engine/RenDevice.h"
#include "Engine/RenManager.h"

template<typename TRenState>
class nsRenStateSelectUndo : public nsBaseAssetSelect {
public:
    nsRenStateSelectUndo() {
        _extensions = {"ggrs"};
        _popupSize = ImVec2(300, 300);
        _device = nsRenDevice::Shared()->Device();
    };

    void Draw(TRenState &state) {
        _current = state;

        nsString path;
        if (_current) {
            path = _device->StateGetPath(_current);
        }

        DrawInputField("Shader", path);

        if (DrawSelectionPopup(path)) {
            if (_current != _selected) {
                nsUndoService::Shared()->Push(new nsUndoVarChange(state, _selected));
            }
        }
    }

protected:
    const char * GetPopupId() override;
    void OnClickBrowse() override;
    void DrawSelectedInfo() override;
    void DrawSelectedPreview() override;
    void OnClickSelectPreview(const nsFilePath &path) override;

private:
    IRenState* _current = nullptr;
    IRenState* _selected = nullptr;
    IRenDevice* _device = nullptr;
};

template<typename TRenState>
const char * nsRenStateSelectUndo<TRenState>::GetPopupId() {
    return "RenStateSelectPopup";
}

template<typename TRenState>
void nsRenStateSelectUndo<TRenState>::OnClickBrowse() {
    _selected = _current;
}

template<typename TRenState>
void nsRenStateSelectUndo<TRenState>::DrawSelectedInfo() {
    if (_selected) {
        _device->StateGetPath(_selected);
        ImGui::Text(_device->StateGetPath(_selected));
    }
}

template<typename TRenState>
void nsRenStateSelectUndo<TRenState>::DrawSelectedPreview() {
}

template<typename TRenState>
void nsRenStateSelectUndo<TRenState>::OnClickSelectPreview(const nsFilePath &path) {
    _selected = _device->StateLoad(path);
}
