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
}

void nsMainMenuBar::Draw() {
    if (ImGui::BeginMainMenuBar()) {
        // Меню File
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New", "Ctrl+N")) {
                // Обработка New
                printf("New project\n");
            }
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
                // Обработка Open
                printf("Open project\n");
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Save", "Ctrl+S")) {
                // Обработка Save
                printf("Save project\n");
            }
            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
                // Обработка Save As
                printf("Save As\n");
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "Alt+F4")) {
                // Выход из приложения
                Sys_Exit();
            }
            ImGui::EndMenu();
        }

        // Меню Edit
        if (ImGui::BeginMenu("Edit")) {
            const auto undo = nsUndoService::Shared();
            if (ImGui::MenuItem("Undo", "Ctrl+Z", false, undo->HasUndo())) {
                undo->Undo();
            }
            if (ImGui::MenuItem("Redo", "Ctrl+Y", false, undo->HasRedo())) {
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
                _model->zoom = 1;
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
