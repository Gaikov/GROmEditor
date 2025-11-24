//
// Created by Roman on 11/18/2025.
//

#include "LibraryView.h"

#include "Core/Package.h"
#include "Core/undo/UndoPropertyChange.h"
#include "Core/undo/UndoService.h"
#include "imgui/imgui.h"
#include "nsLib/FilePath.h"
#include "nsLib/log.h"

nsLibraryView::nsLibraryView() {
    Refresh();
}

void nsLibraryView::Draw() {
    ImGui::Begin("Assets Library");

    if (ImGui::Button("Refresh")) {
        Refresh();
    }

    ImGui::InputText("Search", _filter.AsChar(), nsString::MAX_SIZE - 1);
    ImGui::SameLine();
    if (ImGui::Button("Clear")) {
        _filter = "";
    }

    ImGui::BeginChild("LayoutsLib", ImVec2(0, 300), true, ImGuiWindowFlags_HorizontalScrollbar);
    for (auto file: _files) {
        if (_filter.IsEmpty() || strstr(file.AsChar(), _filter.AsChar())) {
            if (ImGui::Selectable(file.AsChar(), _model->user.currentScene == file.AsChar(), ImGuiSelectableFlags_AllowDoubleClick)) {
                if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                    const std::string value = file.AsChar();
                    nsUndoService::Shared()->Push(new nsUndoPropertyChange(_model->user.currentScene, value));
                    Log::Info("Selected: %s", file.AsChar());
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

void nsLibraryView::Refresh() {
    _files.clear();

    nsFilePath::tList list;

    const nsFilePath path(".");
    path.ListingRecursive(list);

    for (auto item: list) {
        auto ext = item.GetExtension();
        ext.ToLower();
        if (ext == "layout") {
            _files.push_back(item);
        }
    }
}
