//
// Created by Roman on 5/18/2026.
//

#include "NewProjectCommand.h"

#include "models/AppModel.h"
#include "nsLib/locator/ServiceLocator.h"
#include "utils/ProjectUtils.h"
#include "view/alerts/AlertPopup.h"
#include "view/popups/OpenFilePopup.h"
#include "view/popups/PopupsStack.h"
#include <filesystem>

namespace fs = std::filesystem;

namespace {
bool CopyDefaultAssetsToProject(const nsFilePath &projectPath) {
    const fs::path source = fs::absolute("default");
    const fs::path target = fs::absolute(projectPath.AsChar()) / "default";
    std::error_code ec;

    if (!fs::exists(source, ec) || ec) {
        nsAlertPopup::Error("Default project assets folder is not found!");
        return false;
    }

    fs::create_directories(target.parent_path(), ec);
    if (ec) {
        nsAlertPopup::Error("Failed to prepare project folder!");
        return false;
    }

    fs::copy(source, target,
             fs::copy_options::recursive
             | fs::copy_options::overwrite_existing
             | fs::copy_options::directories_only,
             ec);
    if (!ec) {
        fs::copy(source, target,
                 fs::copy_options::recursive
                 | fs::copy_options::overwrite_existing,
                 ec);
    }
    if (ec) {
        nsAlertPopup::Error("Failed to copy default project assets!");
        return false;
    }

    return true;
}
}

void nsNewProjectCommand::Execute(const nsBaseEvent *event) {
    (void) event;

    const auto model = Locate<nsAppModel>();
    const auto popup = nsPopupsStack::Shared()->AddPopup<nsOpenFilePopup>(model->GetProjectPath());
    popup->SetFlags(static_cast<nsOpenFilePopup::Flags>(nsOpenFilePopup::OpenFolder | nsOpenFilePopup::AllowOverwrite | nsOpenFilePopup::Global));
    popup->SetOpenCallback([model](const nsFilePath &path) {
        if (CopyDefaultAssetsToProject(path)) {
            nsProjectUtils::LoadProject(model, path);
        }
    });
}
