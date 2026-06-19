// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file LayoutAssetPolicy.cpp
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#include "LayoutAssetPolicy.h"

#include "Core/undo/UndoBatch.h"
#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVarChange.h"
#include "Engine/display/VisualObject2d.h"
#include "models/AppModel.h"
#include "nsLib/locator/ServiceLocator.h"
#include "nsLib/log.h"
#include <string>

nsLayoutAssetPolicy::nsLayoutAssetPolicy() {
    _model = Locate<nsAppModel>();
}

void nsLayoutAssetPolicy::OnDoubleClick(const nsFilePath &path) {
    Log::Info("Selected: %s", path.AsChar());

    auto &user = _model->project.user;
    const std::string value = path.AsChar();
    const auto batch = new nsUndoBatch();

    batch->Add(new nsUndoVarChange(user.selectedObject,
                                   static_cast<nsVisualObject2d *>(nullptr)));
    batch->Add(new nsUndoVarChange(user.currentScene, value));

    nsUndoService::Shared()->Push(batch);
}

bool nsLayoutAssetPolicy::IsSelected(const nsFilePath &path) const {
    return _model->project.user.currentScene == path.AsChar();
}
