//
// Created by Roman on 11/13/2025.
//

#include "MainMenuBar.h"

#include "alerts/AlertPopup.h"
#include "Core/sys.h"
#include "Core/undo/UndoService.h"
#include "imgui/imgui.h"
#include "nsLib/locator/ServiceLocator.h"
#include "popups/OpenFilePopup.h"

nsMainMenuBar::nsMainMenuBar() {
    _model = Locate<nsAppModel>();

    const auto file = _menu.AddItem("File");
    file->AddItem("New")
            ->Action([] { Log::Info("New project"); })
            ->Shortcut("Ctrl+N", ImGuiMod_Ctrl | ImGuiKey_N);
    file->AddItem("Open")
            ->Action([&] {
                //nsPopupsStack::Shared()->AddPopup<nsFolderSelectDialog>();
            })
            ->Shortcut("Ctrl+O", ImGuiMod_Ctrl | ImGuiKey_O);

    file->AddSeparator();
    file->AddItem("Save")
            ->Shortcut("Ctrl+S", ImGuiMod_Ctrl | ImGuiKey_S)
            ->Action([&] {
                const nsFilePath path = _model->GetProjectPath();
                if (path.IsExists() && path.IsFolder()) {
                    if (!_model->project.Save(path)) {
                        nsAlertPopup::Error("Project save failed!");
                    } else {
                        nsAlertPopup::Info("Project saved!");
                    }
                } else {
                    nsString msg;
                    msg.Format("Project path '%s' is invalid!", path.AsChar());
                    nsAlertPopup::Error(msg);
                }
            });

    file->AddSeparator();
    file->AddItem("Exit")
            ->Action([] { Sys_Exit(); })
            ->Shortcut("Alt+F4", ImGuiMod_Alt | ImGuiKey_F4);

    nsUndoService *undo = nsUndoService::Shared();
    const auto edit = _menu.AddItem("Edit");
    _undo = edit->AddItem("Undo")
            ->Shortcut("Ctrl+Z", ImGuiMod_Ctrl | ImGuiKey_Z)
            ->Action([undo] { undo->Undo(); });
    _redo = edit->AddItem("Redo")
            ->Shortcut("Ctrl+Y", ImGuiMod_Ctrl | ImGuiKey_Y)
            ->Action([undo] { undo->Redo(); });
    edit->AddSeparator();
    edit->AddItem("Cut");
    edit->AddItem("Copy");
    edit->AddItem("Paste");

    auto &user = _model->project.user;
    const auto view = _menu.AddItem("View");
    view->AddItem("Blast Particles")->Action([&] { _model->blastParticles = _model->blastParticles + 1; });
    _xFlip = view->AddItem("Flip X")->Action([&] { user.xFlip = !user.xFlip; });
    _yFlip = view->AddItem("Flip Y")->Action([&] { user.yFlip = !user.yFlip; });
    view->AddItem("Reset Zoom")->Action([&] { user.zoom = 1.0f; });

    const auto debug = _menu.AddItem("Debug");
    _demoView = debug->AddItem("Demo View")->Action([&] { user.testView = !user.testView; });
}

void nsMainMenuBar::Draw() {
    const nsUndoService *undo = nsUndoService::Shared();

    _undo->enabled = undo->HasUndo();
    _redo->enabled = undo->HasRedo();

    const auto &user = _model->project.user;

    _xFlip->selected = user.xFlip;
    _yFlip->selected = user.yFlip;

    _demoView->selected = user.testView;

    _menu.Draw();
    _menu.Update();
}
