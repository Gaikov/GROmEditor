//
// Created by Roman on 12/2/2025.
//

#include "FolderSelectPopup.h"
#include "imgui/imgui.h"
#include "nsLib/StrTools.h"
#include <filesystem>

#include "utils/FileUtils.h"
#include "view/alerts/AlertPopup.h"

nsFolderSelectDialog::nsFolderSelectDialog(const nsFilePath &startPath) : _currentPath(startPath) {
    SetTitle("Browse");
    Refresh();
}

void nsFolderSelectDialog::SetExtensions(const std::vector<std::string> &extensions) {
    _extensions = extensions;
    Refresh();
}

void nsFolderSelectDialog::DrawContent() {
    ImGui::BeginChild("Folders List", ImVec2(400, 300), true, ImGuiWindowFlags_HorizontalScrollbar);
    for (auto &file: _items) {
        if (ImGui::Selectable(file.c_str(), _selectedItem == file.c_str(), ImGuiSelectableFlags_AllowDoubleClick)) {
            UpdateSelected(file.c_str());
            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                if (file == "..") {
                    _currentPath = _currentPath.GetParent();
                } else {
                    _currentPath = _currentPath.ResolvePath(file.c_str());
                }
                _selectedItem = "";
                Refresh();
            }
        }
    }
    ImGui::EndChild();

    ImGui::InputText("File", _selectedItem.AsChar(), nsString::MAX_SIZE - 1);

    if (ImGui::Button("Open")) {
        if (_currentPath.IsEmpty()) {
            nsAlertPopup::Warning("File is not selected!");
        } else {
            //TODO: callback
            ImGui::CloseCurrentPopup();
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
        ImGui::CloseCurrentPopup();
    }
}

void nsFolderSelectDialog::Refresh() {
    _items.clear();

    const auto parent = _currentPath.GetParent();
    if (!parent.IsEmpty()) {
        _items.push_back("..");
    }

    nsFilePath::tList list;
    _currentPath.Listing(list);

    std::sort(list.begin(), list.end(), [](const nsFilePath &a, const nsFilePath &b) {
        if (a.IsFolder() != b.IsFolder()) {
            return a.IsFolder();
        }
        return strcmp(a.GetName(), b.GetName()) < 0;
    });


    for (auto &item: list) {
        if (item.IsFolder()) {
            _items.push_back(item.GetName().AsChar());
        } else if (nsFileUtils::CheckExtension(item, _extensions)) {
            _items.push_back(item.GetName().AsChar());
        }
    }
}

void nsFolderSelectDialog::UpdateSelected(const char *item) {
    _selectedItem = item;
    if (_selectedItem.IsEmpty()) {
        _selectedPath = "";
    } else {
        _selectedPath = _currentPath.ResolvePath(_selectedItem);
    }
}
