//
// Created by Roman on 11/18/2025.
//

#include "LibraryView.h"

#include "Core/Package.h"
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

    ImGui::BeginChild("ScrollingRegion", ImVec2(0, 300), true, ImGuiWindowFlags_HorizontalScrollbar);
    for (auto file: _files) {
        if (_filter.IsEmpty() || strstr(file.AsChar(), _filter.AsChar())) {
            if (ImGui::MenuItem(file.AsChar(), nullptr, _model->user.currentScene == file.AsChar())) {
                Log::Info("Selected: %s", file.AsChar());
                _model->user.currentScene = file.AsChar();
            }
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
