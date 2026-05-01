//
// Created by Roman on 12/2/2025.
//

#include "OpenFilePopup.h"
#include "imgui/imgui.h"
#include "nsLib/StrTools.h"
#include "nsLib/log.h"
#include "utils/FileUtils.h"
#include "view/alerts/AlertPopup.h"

nsOpenFilePopup::nsOpenFilePopup(const nsFilePath &startPath) : _startPath(startPath), _currentPath(startPath) {
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
    if ((_flags & Global) && !_currentPath.IsEmpty()) {
        _currentPath = _currentPath.ToAbsolute();
    }
    Refresh();
}

void nsOpenFilePopup::DrawContent() {
    DrawFolderCreation();

    if (_flags & Global) {
        ImGui::BeginChild("Drives", ImVec2(60, 300), ImGuiChildFlags_Borders);
        for (char c = 'A'; c <= 'Z'; c++) {
            char drive[] = {c, ':', '/', '\0'};
            if (nsFilePath::Exists(drive)) {
                const bool active = strncmp(_currentPath.AsChar(), drive, 3) == 0;
                if (ImGui::Selectable(drive, active)) {
                    _currentPath = nsFilePath(drive);
                    _selectedItem = "";
                    Refresh();
                }
            }
        }
        ImGui::EndChild();
        ImGui::SameLine();
    }

    ImGui::BeginChild("Folders List", ImVec2(400, 300),
        ImGuiChildFlags_Borders,
        ImGuiWindowFlags_HorizontalScrollbar);
    for (auto &file: _items) {
        if (ImGui::Selectable(file.c_str(), _selectedItem == file.c_str(), ImGuiSelectableFlags_AllowDoubleClick)) {
            UpdateSelected(file.c_str());
            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                if (file == "..") {
                    const char *cur = _currentPath.AsChar();
                    const int curLen = (int)strlen(cur);
                    if ((_flags & Global) && curLen == 3 && cur[1] == ':' && cur[2] == '/') {
                        _currentPath = nsFilePath("");
                    } else {
                        const nsFilePath parent = _currentPath.GetParent();
                        const char *p = parent.AsChar();
                        if (strlen(p) == 2 && p[1] == ':') {
                            char driveRoot[] = {p[0], ':', '/', '\0'};
                            _currentPath = nsFilePath(driveRoot);
                        } else {
                            _currentPath = parent;
                        }
                    }
                    _selectedItem = "";
                    Refresh();
                    break;
                } else {
                    nsFilePath path = _currentPath.ResolvePath(file.c_str());
                    if (path.IsFolder()) {
                        _currentPath = path;
                        _selectedItem = "";
                        Refresh();
                        break;
                    } //else if allow overwrite
                }
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

void nsOpenFilePopup::DrawFolderCreation() {
    if (ImGui::Button("Create Folder")) {
        _folderNewName = "";
        ImGui::OpenPopup("Create Folder");
    }

    if (ImGui::BeginPopup("Create Folder")) {
        ImGui::InputText("Folder Name", _folderNewName.AsChar(), nsString::MAX_SIZE - 1);
        const nsFilePath path = _currentPath.ResolvePath(_folderNewName);


        const bool exists = path.IsExists();
        if (_folderNewName.IsEmpty()) {
            ImGui::Text("Enter folder name.");
        }
        else if (exists) {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Folder already exists!");
        }

        if (!_folderNewName.IsEmpty() && !exists) {
            if (ImGui::Button("Create")) {
                ImGui::CloseCurrentPopup();
                if (!path.CreateFolders()) {
                    nsAlertPopup::Error("Failed to create folder!");
                } else {
                    Refresh();
                }
            }
        }
        ImGui::EndPopup();
    }
}

void nsOpenFilePopup::Refresh() {
    _items.clear();

    if (_currentPath.IsEmpty()) return;

    if (_flags & Global) {
        _items.push_back("..");
    } else {
        if (!(_currentPath == _startPath)) {
            _items.push_back("..");
        }
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
        } else if (!(_flags & OpenFolder) && nsFileUtils::CheckExtension(item, _extensions)) {
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
