//
// Created by Roman on 11/13/2025.
//

#include "MainMenuBar.h"

#include "Core/sys.h"
#include "Core/undo/UndoService.h"
#include "imgui/imgui.h"
#include "nsLib/locator/ServiceLocator.h"

nsMainMenuBar::nsMainMenuBar() {
    _model = Locate<nsSVModel>();

    const auto file = _menu.AddItem("File");
    file->AddItem("New")
            ->Action([] { Log::Info("New project"); })
            ->Shortcut("Ctrl+N", ImGuiMod_Ctrl | ImGuiKey_N);
    file->AddItem("Open")
            ->Action([] { Log::Info("Open project"); })
            ->Shortcut("Ctrl+O", ImGuiMod_Ctrl | ImGuiKey_O);

    file->AddSeparator();
    file->AddItem("Save")
            ->Shortcut("Ctrl+S", ImGuiMod_Ctrl | ImGuiKey_S)
            ->Action([] { Log::Info("Save project"); });

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

    auto &user = _model->user;
    const auto view = _menu.AddItem("View");
    _emitParticles = view->AddItem("Emit Particles")->Action([&] { user.emitParticles = !user.emitParticles; });
    view->AddItem("Blast Particles")->Action([&] { _model->blastParticles = _model->blastParticles + 1; });
    _xFlip = view->AddItem("Flip X")->Action([&] { user.xFlip = !user.xFlip; });
    _yFlip = view->AddItem("Flip Y")->Action([&] { user.yFlip = !user.yFlip; });
    view->AddItem("Reset Zoom")->Action([&] { _model->user.zoom = 1.0f; });

    const auto debug = _menu.AddItem("Debug");
    _demoView = debug->AddItem("Demo View")->Action([&] { user.testView = !user.testView; });
}

void nsMainMenuBar::Draw() {
    const nsUndoService *undo = nsUndoService::Shared();

    _undo->enabled = undo->HasUndo();
    _redo->enabled = undo->HasRedo();

    _emitParticles->selected = _model->user.emitParticles;
    _xFlip->selected = _model->user.xFlip;
    _yFlip->selected = _model->user.yFlip;

    _demoView->selected = _model->user.testView;

    _menu.Draw();
    _menu.Update();
}
