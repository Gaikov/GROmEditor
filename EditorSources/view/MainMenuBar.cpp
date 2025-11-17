//
// Created by Roman on 11/13/2025.
//

#include "MainMenuBar.h"

#include "alerts/AlertPopup.h"
#include "Core/sys.h"
#include "imgui/imgui.h"
#include "nsLib/locator/ServiceLocator.h"
#include "view/popups/PopupsStack.h"

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
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
                printf("Undo\n");
            }
            if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false)) {
                // Disabled item
            }
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
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("Emit Particles")) {
                _model->emitParticles = !_model->emitParticles;
            }
            if (ImGui::MenuItem("Blast Particles")) {
                nsAlertPopup::Warning("Not Implemented!");
            }
            if (ImGui::MenuItem("Flip X")) {
                _model->xFlip = !_model->xFlip;
            }
            if (ImGui::MenuItem("Flip Y")) {
                _model->yFlip = !_model->yFlip;
            }
            if (ImGui::MenuItem("Reset Zoom")) {
                _model->zoom = 1;
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Debug")) {
            if (ImGui::MenuItem("Demo View", nullptr, _model->testView)) {
                _model->testView = !_model->testView;
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
