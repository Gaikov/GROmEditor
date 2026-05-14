//
// Created by Roman on 12/8/2025.
//

#include "BaseAssetSelect.h"

#include <cstring>

#include "imgui/imgui.h"
#include "models/AppModel.h"
#include "nsLib/locator/ServiceLocator.h"

nsBaseAssetSelect::nsBaseAssetSelect() : _model(Locate<nsAppModel>()) {
}

void nsBaseAssetSelect::DrawInputField(const char *title, const char *currentPath) {
    CacheDisplayPath(currentPath, _inputPathSource, _inputPathDisplay);
    ImGui::InputText(title, _inputPathDisplay.AsChar(), nsString::MAX_SIZE - 1, ImGuiInputTextFlags_ReadOnly);
    ImGui::SameLine();

    nsString buttonId;
    buttonId.Format("Browse##%s", GetPopupId());

    if (ImGui::Button(buttonId)) {
        OnClickBrowse();

        const auto projectPath = _model->GetProjectPath();
        _files.clear();
        _displayFiles.clear();
        std::vector<nsFilePath> files;
        projectPath.ListingRecursive(files);
        for (auto &item: files) {
            if (HasValidExtension(item)) {
                _files.push_back(item);
                _displayFiles.push_back(projectPath.GetRelativePath(item));
            }
        }

        ImGui::OpenPopup(GetPopupId());
    }
}

bool nsBaseAssetSelect::DrawSelectionPopup(const nsString &path) {
    bool selected = false;
    if (ImGui::BeginPopup(GetPopupId(), ImGuiWindowFlags_AlwaysAutoResize)) {
        CacheDisplayPath(path, _popupPathSource, _popupPathDisplay);
        ImGui::InputText("Search", _filter.AsChar(), nsString::MAX_SIZE - 1);
        ImGui::SameLine();
        if (ImGui::Button("Clear")) {
            _filter = "";
        }

        DrawSelectedInfo();

        ImGui::BeginChild("Assets List", _popupSize,
            ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_Borders,
            ImGuiWindowFlags_HorizontalScrollbar);
        for (size_t i = 0; i < _files.size(); ++i) {
            const auto &file = _files[i];
            const auto &display = _displayFiles[i];
            const char *displayStr = display;
            if (_filter.IsEmpty() || strstr(displayStr, _filter.AsChar())) {
                if (ImGui::Selectable(displayStr, _popupPathDisplay == displayStr, ImGuiSelectableFlags_AllowDoubleClick)) {
                    Log::Info("click: %s", file.AsChar());
                    OnClickSelectPreview(file);

                    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                        Log::Info("Selected: %s", file.AsChar());
                        selected = true;
                    }
                }
            }
        }
        ImGui::EndChild();
        ImGui::SameLine();

        DrawSelectedPreview();

        ImGui::EndPopup();
    }
    return selected;
}

bool nsBaseAssetSelect::HasValidExtension(const nsFilePath &path) const {
    if (_extensions.empty()) {
        return true;
    }

    auto ext = path.GetExtension();
    ext.ToLower();
    for (auto &i : _extensions) {
        if (ext == i.c_str()) {
            return true;
        }
    }
    return false;
}

const char *nsBaseAssetSelect::GetInfoDisplayPath(const char *path) {
    CacheDisplayPath(path, _infoPathSource, _infoPathDisplay);
    return _infoPathDisplay.AsChar();
}

void nsBaseAssetSelect::CacheDisplayPath(const char *source, nsString &cachedSource, nsString &cachedDisplay) const {
    const char *safeSource = source ? source : "";
    if (cachedSource == safeSource) {
        return;
    }

    cachedSource = safeSource;
    cachedDisplay = safeSource;
    if (cachedDisplay.IsEmpty() || !_model) {
        return;
    }

    if (!nsFilePath::IsAbsolute(cachedDisplay)) {
        return;
    }

    const nsFilePath projectPath = _model->GetProjectPath().ToAbsolute();
    const nsFilePath fullPath = nsFilePath(cachedDisplay).ToAbsolute();
    const char *projectStr = projectPath.AsChar();
    const char *fullStr = fullPath.AsChar();
    const size_t projectLen = strlen(projectStr);

    if (strncmp(fullStr, projectStr, projectLen) != 0) {
        return;
    }

    const char suffix = fullStr[projectLen];
    if (suffix != 0 && suffix != '/') {
        return;
    }

    cachedDisplay = projectPath.GetRelativePath(fullPath);
}
