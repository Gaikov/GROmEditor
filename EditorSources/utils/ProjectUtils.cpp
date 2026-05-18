//
// Created by Roman on 5/18/2026.
//

#include "ProjectUtils.h"

#include "view/alerts/AlertPopup.h"

bool nsProjectUtils::LoadProject(nsAppModel *model, const nsFilePath &path) {
    model->settings.projectPath = path.AsChar();
    if (!model->project.Load(path)) {
        nsAlertPopup::Error("Failed to open project!");
        return false;
    }

    model->Save();
    return true;
}
