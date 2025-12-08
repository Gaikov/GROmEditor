//
// Created by Roman on 12/8/2025.
//

#include "BaseAssetSelect.h"

#include "imgui/imgui.h"
#include "models/SVModel.h"
#include "nsLib/locator/ServiceLocator.h"

void nsBaseAssetSelect::DrawInputField(const char *title, const char *currentPath) {
    nsString path = currentPath;
    ImGui::InputText(title, path.AsChar(), nsString::MAX_SIZE - 1, ImGuiInputTextFlags_ReadOnly);
    ImGui::SameLine();
    if (ImGui::Button("Browse")) {
        OnClickBrowse();

        const auto model = Locate<nsSVModel>();
        const auto projectPath = model->GetProjectPath();
        _files.clear();
        std::vector<nsFilePath> files;
        projectPath.ListingRecursive(files);
        for (auto &item: files) {
            if (HasValidExtension(item)) {
                _files.push_back(item);
            }
        }

        ImGui::OpenPopup(GetPopupId());
    }
}

bool nsBaseAssetSelect::DrawSelectionPopup(const nsString &path) {
    bool selected = false;
    if (ImGui::BeginPopup(GetPopupId(), ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::InputText("Search", _filter.AsChar(), nsString::MAX_SIZE - 1);
        ImGui::SameLine();
        if (ImGui::Button("Clear")) {
            _filter = "";
        }

        DrawSelectedInfo();


        ImGui::BeginChild("Assets List", _popupSize,
            ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_Borders,
            ImGuiWindowFlags_HorizontalScrollbar);
        for (auto file: _files) {
            if (_filter.IsEmpty() || strstr(file.AsChar(), _filter.AsChar())) {
                if (ImGui::Selectable(file.AsChar(), path == file.AsChar(), ImGuiSelectableFlags_AllowDoubleClick)) {
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
