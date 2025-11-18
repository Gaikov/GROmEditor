//
// Created by Roman on 11/18/2025.
//

#include "LibraryView.h"

#include "Core/Package.h"
#include "Core/Var.h"
#include "imgui/imgui.h"
#include "nsLib/FilePath.h"
#include "nsLib/log.h"

extern  nsVar    *sv_last_layout;

nsLibraryView::nsLibraryView() {
    Refresh();
}

void nsLibraryView::Draw() {
    ImGui::Begin("Assets Library");

    if (ImGui::Button("Refresh")) {
        Refresh();
    }

    ImGui::BeginChild("ScrollingRegion", ImVec2(0, 300), true, ImGuiWindowFlags_HorizontalScrollbar);
    for (auto file : _files) {
        if (ImGui::MenuItem(file.AsChar())) {
            Log::Info("Selected: %s", file.AsChar());
            sv_last_layout->SetString(file.AsChar());
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

    for (auto item : list) {
        auto ext = item.GetExtension();
        ext.ToLower();
        if (ext == "layout") {
            _files.push_back(item);
        }
    }
}
