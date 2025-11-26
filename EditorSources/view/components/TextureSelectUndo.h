// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file TextureSelectUndo.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
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
class nsTextureSelectUndo {
public:
    static constexpr auto POPUP_ID = "Texture selection";
    static constexpr auto IMAGE_SIZE = 300;

public:
    nsTextureSelectUndo(const char *title) : _title(title), _device(nsRenDevice::Shared()->Device()) {
    }

    void Draw(TTexture &var) {
        //TODO: relative project path
        ITexture *tex = var;

        nsString path = _device->TextureGetPath(tex);
        ImGui::InputText(_title.c_str(), path.AsChar(), nsString::MAX_SIZE - 1, ImGuiInputTextFlags_ReadOnly);
        ImGui::SameLine();
        if (ImGui::Button("Browse")) {
            _current = tex;
            const auto model = Locate<nsSVModel>();
            const auto projectPath = model->GetProjectPath();
            _files.clear();
            std::vector<nsFilePath> files;
            projectPath.ListingRecursive(files);
            for (auto &item: files) {
                auto ext = item.GetExtension();
                ext.ToLower();
                if (ext == "png" || ext == "jpg") {
                    _files.push_back(item);
                }
            }

            ImGui::OpenPopup(POPUP_ID);
        }

        if (ImGui::BeginPopup(POPUP_ID, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::BeginChild("Images", ImVec2(0, IMAGE_SIZE),
                ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeX, ImGuiWindowFlags_HorizontalScrollbar);
            for (auto file: _files) {
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
            ImGui::EndChild();
            ImGui::SameLine();

            const auto id = _current ? _current->GetId() : 0;
            ImGui::Image(static_cast<intptr_t>(id), ImVec2(IMAGE_SIZE, IMAGE_SIZE));

            ImGui::EndPopup();
        }
    }

private:
    std::string _title;
    IRenDevice *_device;
    ITexture *_current = nullptr;

    std::vector<nsFilePath> _files;
};
