// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file PmaCheckboxUndo.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include <cstring>
#include <string>

#include "Core/undo/UndoBatch.h"
#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVarChange.h"
#include "Engine/RenDevice.h"
#include "Engine/RenManager.h"
#include "imgui/imgui.h"
#include "models/AppModel.h"
#include "nsLib/locator/ServiceLocator.h"

template<typename TFlag, typename TTexture, typename TRenState>
class nsPmaCheckboxUndo {
public:
    explicit nsPmaCheckboxUndo(const char *title) : _title(title) {
    }

    void Draw(TFlag &flag, TTexture &texture, TRenState &renState) {
        DrawField(_title.c_str(), flag, texture, renState);
    }

    static void DrawField(const char *title, TFlag &flag, TTexture &texture, TRenState &renState) {
        bool value = flag;
        if (!ImGui::Checkbox(title, &value)) {
            return;
        }

        const auto model = Locate<nsAppModel>();
        const auto device = nsRenDevice::Shared()->Device();
        const nsFilePath pmaStatePath = model->GetProjectPath().ResolvePath("default/rs/sprite_pma.ggrs");
        const nsFilePath defaultStatePath = model->GetProjectPath().ResolvePath("default/rs/gui.ggrs");
        const int flags = value ? TLF_PREMULTIPLY_ALPHA : TLF_NONE;

        ITexture *nextTexture = texture;
        if (nextTexture) {
            const nsString texturePath = device->TextureGetPath(nextTexture);
            nextTexture = texturePath.IsEmpty()
                          ? nullptr
                          : device->TextureLoad(texturePath, false, TF_RGBA, flags);
        }

        IRenState *nextState = renState;
        if (value) {
            nextState = device->StateLoad(pmaStatePath);
        } else if (nextState && device->StateGetPath(nextState)
                   && strcmp(device->StateGetPath(nextState), pmaStatePath) == 0) {
            nextState = device->StateLoad(defaultStatePath);
        }

        const auto batch = new nsUndoBatch();
        batch->Add(new nsUndoVarChange(flag, value));
        if (nextTexture != texture) {
            batch->Add(new nsUndoVarChange(texture, nextTexture));
        }
        if (nextState != renState) {
            batch->Add(new nsUndoVarChange(renState, nextState));
        }
        nsUndoService::Shared()->Push(batch);
    }

private:
    std::string _title;
};
