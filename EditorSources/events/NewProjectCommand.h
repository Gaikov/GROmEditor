// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file NewProjectCommand.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "EditorCommand.h"
#include "models/AppModel.h"
#include "nsLib/locator/ServiceLocator.h"
#include "utils/ProjectUtils.h"
#include "view/popups/OpenFilePopup.h"
#include "view/popups/PopupsStack.h"
#include "view/alerts/AlertPopup.h"
#include <filesystem>

class nsNewProjectCommand final : public nsIEditorCommand {
public:
    void Execute(const nsBaseEvent *event) override {
        (void) event;

        namespace fs = std::filesystem;

        const auto model = Locate<nsAppModel>();
        const auto popup = nsPopupsStack::Shared()->AddPopup<nsOpenFilePopup>(model->GetProjectPath());
        popup->SetFlags(static_cast<nsOpenFilePopup::Flags>(nsOpenFilePopup::OpenFolder | nsOpenFilePopup::AllowOverwrite | nsOpenFilePopup::Global));
        popup->SetOpenCallback([model](const nsFilePath &path) {
            const fs::path source = fs::absolute("default");
            const fs::path target = fs::absolute(path.AsChar()) / "default";
            std::error_code ec;

            if (!fs::exists(source, ec) || ec) {
                nsAlertPopup::Error("Default project assets folder is not found!");
                return;
            }

            fs::create_directories(target.parent_path(), ec);
            if (ec) {
                nsAlertPopup::Error("Failed to prepare project folder!");
                return;
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
                return;
            }

            nsProjectUtils::LoadProject(model, path);
        });
    }
};
