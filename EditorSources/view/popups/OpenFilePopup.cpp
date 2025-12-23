//
// Created by Roman on 12/2/2025.
//

#include "OpenFilePopup.h"
#include "imgui/imgui.h"
#include "nsLib/StrTools.h"
#include "nsLib/log.h"
#include "utils/FileUtils.h"
#include "view/alerts/AlertPopup.h"

nsOpenFilePopup::nsOpenFilePopup(const nsFilePath &startPath) : _currentPath(startPath) {
    SetTitle("Browse");
    Refresh();
}

void nsOpenFilePopup::SetExtensions(const std::vector<std::string> &extensions) {
    _extensions = extensions;
    Refresh();
}

void nsOpenFilePopup::SetOpenCallback(const OpenCallback_t &callback) {
    _callback = callback;
}

void nsOpenFilePopup::SetFlags(const Flags flags) {
    _flags = flags;
}

void nsOpenFilePopup::DrawContent() {
    ImGui::BeginChild("Folders List", ImVec2(400, 300),
        //ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY,
        ImGuiChildFlags_Borders,
        ImGuiWindowFlags_HorizontalScrollbar);
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

    if (ImGui::InputText("File", _selectedItem.AsChar(), nsString::MAX_SIZE - 1)) {
        UpdateSelected(_selectedItem.AsChar());
    }

    if (ImGui::Button("Open")) {
        if (ValidatePath()) {
            Log::Info("Selected file path: %s", _selectedPath.AsChar());
            _callback(_selectedPath);
            ImGui::CloseCurrentPopup();
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
        ImGui::CloseCurrentPopup();
    }
}

void nsOpenFilePopup::Refresh() {
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

void nsOpenFilePopup::UpdateSelected(const char *item) {
    _selectedItem = item;
    if (_selectedItem.IsEmpty()) {
        _selectedPath = "";
    } else {
        _selectedPath = _currentPath.ResolvePath(_selectedItem);
        if (!(_flags & OpenFolder)) {
            _selectedPath = nsFileUtils::EnsureExtension(_selectedPath, _extensions);
        }
    }
}

bool nsOpenFilePopup::ValidatePath() const {
    if (_selectedPath.IsEmpty()) {
        nsAlertPopup::Warning("File is not selected!");
        return false;
    }

    if (_flags & OpenFolder) {
        if (!_selectedPath.IsFolder()) {
            return false;
        }
    } else {
        if (_selectedPath.IsFolder()) {
            return false;
        }
    }

    if (!(_flags & AllowOverwrite)) {
        if (_selectedPath.IsExists()) {
            nsString msg;
            msg.Format("File or folder already exists:\n%s", _selectedPath.AsChar());
            nsAlertPopup::Warning(msg);
            return false;
        }
    }

    if (!_callback) {
        nsAlertPopup::Error("Open callback is not set!");
        return false;
    }

    return true;
}
