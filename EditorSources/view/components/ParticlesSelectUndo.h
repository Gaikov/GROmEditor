// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ParticlesSelectUndo.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "BaseAssetSelect.h"
#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVarChange.h"
#include "Engine/renderer/particles/ParticlesManager.h"
#include "Engine/renderer/particles/ParticleSystem.h"

template <typename TParticlesVar>
class nsParticlesSelectUndo : public nsBaseAssetSelect {
public:
    nsParticlesSelectUndo() {
        _extensions = {"ggps"};
        _popupSize = ImVec2(300, 300);
    };

    void Draw(TParticlesVar &var) {
        _current = var;

        nsString path;
        if (_current) {
            path = nsParticlesManager::Shared()->GetParticlesPath(_current);
        }

        DrawInputField("Asset", path);

        if (DrawSelectionPopup(path)) {
            if (_current != _selected) {
                nsUndoService::Shared()->Push(new nsUndoVarChange(var, _selected));
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
    nsParticlesBehaviour *_current = nullptr;
    nsParticlesBehaviour *_selected = nullptr;
};

template<typename TParticlesVar>
const char * nsParticlesSelectUndo<TParticlesVar>::GetPopupId() {
    return "ParticlesSelectPopup";
}

template<typename TParticlesVar>
void nsParticlesSelectUndo<TParticlesVar>::OnClickBrowse() {
    _selected = _current;
}

template<typename TParticlesVar>
void nsParticlesSelectUndo<TParticlesVar>::DrawSelectedInfo() {
    if (_selected) {
        ImGui::Text(nsParticlesManager::Shared()->GetParticlesPath(_selected));
    }
}

template<typename TParticlesVar>
void nsParticlesSelectUndo<TParticlesVar>::DrawSelectedPreview() {
}

template<typename TParticlesVar>
void nsParticlesSelectUndo<TParticlesVar>::OnClickSelectPreview(const nsFilePath &path) {
    _selected = nsParticlesManager::Shared()->LoadParticles(path);
}
