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
}

void nsMainMenuBar::Draw() {
    nsUndoService *undo = Locate<nsUndoService>();

    _menu.Draw();
    _menu.Update();
    return;

    if (ImGui::BeginMainMenuBar()) {


        // Меню Edit
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z", false, undo->HasUndo())) {
                Log::Debug("edit->undo");
                undo->Undo();
            }
            if (ImGui::MenuItem("Redo", "Ctrl+Y", false, undo->HasRedo())) {
                Log::Debug("edit->redo");
                undo->Redo();
            }
            /*
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "Ctrl+X")) {
                printf("Cut\n");
            }
            if (ImGui::MenuItem("Copy", "Ctrl+C")) {
                printf("Copy\n");
            }
            if (ImGui::MenuItem("Paste", "Ctrl+V")) {
                printf("Paste\n");
            }
            */
            ImGui::EndMenu();
        }

        auto &user = _model->user;
        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("Emit Particles", nullptr, user.emitParticles)) {
                user.emitParticles = !user.emitParticles;
            }
            if (ImGui::MenuItem("Blast Particles")) {
                _model->blastParticles = _model->blastParticles + 1;
            }
            if (ImGui::MenuItem("Flip X", nullptr, user.xFlip)) {
                user.xFlip = !user.xFlip;
            }
            if (ImGui::MenuItem("Flip Y", nullptr, user.yFlip)) {
                user.yFlip = !user.yFlip;
            }
            if (ImGui::MenuItem("Reset Zoom")) {
                _model->user.zoom = 1.0f;
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Debug")) {
            if (ImGui::MenuItem("Demo View", nullptr, user.testView)) {
                user.testView = !user.testView;
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
