// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file TextureSelectUndo.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "BaseAssetSelect.h"
#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVarChange.h"
#include "Engine/RenDevice.h"
#include "Engine/RenManager.h"
#include "imgui/imgui.h"
#include "nsLib/StrTools.h"

template<typename TTexture>
class nsTextureSelectUndo : public nsBaseAssetSelect {
public:
    static constexpr auto IMAGE_SIZE = 300;

    nsTextureSelectUndo(const char *title) : _title(title), _device(nsRenDevice::Shared()->Device()) {
        _extensions = { "png", "jpg" };
        _popupSize = { IMAGE_SIZE * 1.5f, IMAGE_SIZE };
        _popupId = _title + "Texture selection";
    }

    void Draw(TTexture &var) {
        //TODO: relative project path
        _passedTexture = var;
        const nsString path = _device->TextureGetPath(_passedTexture);
        DrawInputField(_title.c_str(), path);

        if (DrawSelectionPopup(path)) {
            nsUndoService::Shared()->Push(new nsUndoVarChange(var, _current));
        }
    }

protected:
    void DrawSelectedInfo() override {
        if (_current) {
            int w, h;
            _current->GetSize(w, h);
            ImGui::Text("%s [%dx%d]", _device->TextureGetPath(_current), w, h);
        }
    }

    void DrawSelectedPreview() override {
        const auto id = _current ? _current->GetId() : 0;
        ImGui::Image(static_cast<intptr_t>(id), ImVec2(IMAGE_SIZE, IMAGE_SIZE));
    }

    void OnClickSelectPreview(const nsFilePath &path) override {
        _current = _device->TextureLoad(path);
        _device->TextureBind(_current);
    }

    const char * GetPopupId() override {
        return _popupId.c_str();
    }

    void OnClickBrowse() override {
        _current = _passedTexture;
    }

private:
    std::string _title;
    std::string _popupId;
    IRenDevice *_device;
    ITexture *_current = nullptr;
    ITexture *_passedTexture = nullptr;
};
