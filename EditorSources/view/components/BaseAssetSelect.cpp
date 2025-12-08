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
