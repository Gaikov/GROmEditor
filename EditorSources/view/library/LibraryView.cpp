//
// Created by Roman on 11/18/2025.
//

#include "LibraryView.h"

#include "Core/Package.h"
#include "Core/undo/UndoBatch.h"
#include "Core/undo/UndoPropertyChange.h"
#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVarChange.h"
#include "imgui/imgui.h"
#include "nsLib/log.h"

nsLibraryView::nsLibraryView() {
}

void nsLibraryView::Draw() {
    ImGui::Begin("Assets Library");

    ImGui::InputText("Search", _filter.AsChar(), nsString::MAX_SIZE - 1);
    ImGui::SameLine();
    if (ImGui::Button("Clear")) {
        _filter = "";
    }

    auto &user = _model->project.user;

    ImGui::BeginChild("LayoutsLib", ImVec2(0, 300), true, ImGuiWindowFlags_HorizontalScrollbar);
    for (auto file: _model->project.scenes.GetFiles()) {
        if (_filter.IsEmpty() || strstr(file.AsChar(), _filter.AsChar())) {
            if (ImGui::Selectable(file.AsChar(), user.currentScene == file.AsChar(), ImGuiSelectableFlags_AllowDoubleClick)) {
                if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                    Log::Info("Selected: %s", file.AsChar());
                    const std::string value = file.AsChar();
                    const auto undo = nsUndoService::Shared();
                    const auto batch = new nsUndoBatch();

                    batch->Add(new nsUndoVarChange(user.selectedObject, static_cast<nsVisualObject2d *>(nullptr)));
                    batch->Add(new nsUndoVarChange(user.currentScene, value));

                    undo->Push(batch);
                }
            }

            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                const char* data = file.AsChar();
                ImGui::SetDragDropPayload("MY_DND_TYPE", data, strlen(data) + 1);
                ImGui::Text("Drop to scene tree...");
                ImGui::EndDragDropSource();
            }//*/
        }
    }
    ImGui::EndChild();

    ImGui::End();
}