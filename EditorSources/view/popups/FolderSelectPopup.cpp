//
// Created by Roman on 12/2/2025.
//

#include "FolderSelectPopup.h"
#include "imgui/imgui.h"
#include "nsLib/StrTools.h"
#include <filesystem>

nsFolderSelectDialog::nsFolderSelectDialog() : _currentPath(std::filesystem::current_path().string().c_str()) {
    SetTitle("Select Folder");
    Refresh();
}

void nsFolderSelectDialog::DrawContent() {
    ImGui::BeginChild("Folders List", ImVec2(400, 300), true, ImGuiWindowFlags_HorizontalScrollbar);
    for (auto file: _folders) {
        if (ImGui::Selectable(file.c_str(), _selectedFolder == file, ImGuiSelectableFlags_AllowDoubleClick)) {
            _selectedFolder = file;
            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            }
        }
    }
    ImGui::EndChild();

    if (ImGui::Button("Open")) {
        ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
        ImGui::CloseCurrentPopup();
    }
}

void nsFolderSelectDialog::Refresh() {
    _folders.clear();

    const auto parent = _currentPath.GetParent();
    if (strlen(parent)) {
        _folders.push_back("..");
    }

    nsFilePath::tList list;
    _currentPath.Listing(list);
    for (auto &item: list) {
        if (item.IsFolder()) {
            _folders.push_back(item.GetName().AsChar());
        }
    }
}
