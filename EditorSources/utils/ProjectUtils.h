// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ProjectUtils.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "models/AppModel.h"
#include "view/alerts/AlertPopup.h"

class nsProjectUtils final {
public:
    static bool LoadProject(nsAppModel *model, const nsFilePath &path) {
        model->settings.projectPath = path.AsChar();
        if (!model->project.Load(path)) {
            nsAlertPopup::Error("Failed to open project!");
            return false;
        }

        model->Save();
        return true;
    }
};
