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
#include "models/SVModel.h"
#include "nsLib/FilePath.h"
#include "nsLib/StrTools.h"
#include "nsLib/locator/ServiceLocator.h"

template<typename TTexture>
class nsTextureSelectUndo : public nsBaseAssetSelect {
public:
    static constexpr auto POPUP_ID = "Texture selection";
    static constexpr auto IMAGE_SIZE = 300;

public:
    nsTextureSelectUndo(const char *title) : _title(title), _device(nsRenDevice::Shared()->Device()) {
        _extensions = { "png", "jpg" };
    }

    void Draw(TTexture &var) {
        //TODO: relative project path
        _passedTexture = var;
        const nsString path = _device->TextureGetPath(_passedTexture);
        DrawInputField(_title.c_str(), path);


        if (ImGui::BeginPopup(GetPopupId(), ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::InputText("Search", _filter.AsChar(), nsString::MAX_SIZE - 1);
            ImGui::SameLine();
            if (ImGui::Button("Clear")) {
                _filter = "";
            }

            if (_current) {
                int w, h;
                _current->GetSize(w, h);
                ImGui::Text("%s [%dx%d]", _device->TextureGetPath(_current), w, h);
            }

            ImGui::BeginChild("Images", ImVec2(IMAGE_SIZE * 1.5f, IMAGE_SIZE),
                ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_Borders,
                ImGuiWindowFlags_HorizontalScrollbar);
            for (auto file: _files) {
                if (_filter.IsEmpty() || strstr(file.AsChar(), _filter.AsChar())) {
                    if (ImGui::Selectable(file.AsChar(), path == file.AsChar(), ImGuiSelectableFlags_AllowDoubleClick)) {
                        Log::Info("click: %s", file.AsChar());
                        _current = _device->TextureLoad(file.AsChar());
                        _device->TextureBind(_current);

                        if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                            Log::Info("Selected: %s", file.AsChar());
                            nsUndoService::Shared()->Push(new nsUndoVarChange(var, _current));
                        }
                    }
                }
            }
            ImGui::EndChild();
            ImGui::SameLine();

            const auto id = _current ? _current->GetId() : 0;
            ImGui::Image(static_cast<intptr_t>(id), ImVec2(IMAGE_SIZE, IMAGE_SIZE));

            ImGui::EndPopup();
        }
    }

protected:
    const char * GetPopupId() override {
        return POPUP_ID;
    }

    void OnClickBrowse() override {
        _current = _passedTexture;
    }

private:
    std::string _title;
    IRenDevice *_device;
    ITexture *_current = nullptr;
    ITexture *_passedTexture = nullptr;

    nsString _filter;
};
