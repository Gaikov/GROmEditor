//
// Created by Roman on 11/18/2025.
//

#include "LibraryView.h"

#include "SceneTreeView.h"
#include "Core/Package.h"
#include "Core/undo/UndoBatch.h"
#include "Core/undo/UndoPropertyChange.h"
#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVarChange.h"
#include "Core/undo/file/UndoFileCreate.h"
#include "Engine/display/container/VisualContainer2d.h"
#include "imgui/imgui.h"
#include "nsLib/log.h"
#include "utils/FileUtils.h"
#include "view/popups/OpenFilePopup.h"
#include "view/popups/PopupsStack.h"
#include <algorithm>
#include <cstring>
#include <string>

nsLibraryView::nsLibraryView() {

    _model->settings.projectPath.AddHandler(nsPropChangedName::CHANGED, [&](const nsBaseEvent *) {
        RefreshAssetsTree();
    });
    RefreshAssetsTree();
}

void nsLibraryView::Draw() {
    ImGui::Begin("Assets Library");

    if (ImGui::CollapsingHeader("Layouts Library", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::InputText("Search", _filter.AsChar(), nsString::MAX_SIZE - 1);
        ImGui::SameLine();
        if (ImGui::Button("Clear")) {
            _filter = "";
        }
        ImGui::SameLine();
        if (ImGui::Button("Refresh")) {
            RefreshAssetsTree();
        }

        auto &user = _model->project.user;

        ImGui::BeginChild("LayoutsLib", ImVec2(0, 300),
                          ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeY,
                          ImGuiWindowFlags_HorizontalScrollbar);
        for (auto &node: _assetsTree.children) {
            if (IsAssetTreeNodeVisible(node)) {
                DrawAssetsTreeNode(node);
            }
        }
        ImGui::EndChild();

        if (ImGui::Button("Create")) {
            auto path = _model->GetProjectPath();
            const auto popup = nsPopupsStack::Shared()->AddPopup<nsOpenFilePopup>(path);
            popup->SetTitle("Select layout file");
            popup->SetExtensions({"layout"});
            popup->SetOpenCallback([this](const nsFilePath &layoutPath) {
                nsUndoService::Shared()->Push(new nsUndoCreateLayout(layoutPath, new nsVisualContainer2d()));
                RefreshAssetsTree();
            });
        }
    }

    ImGui::End();
}

void nsLibraryView::RefreshAssetsTree() {
    _assetsTree.children.clear();
    const auto projectPath = _model->GetProjectPath();
    if (projectPath.IsFolder()) {
        BuildAssetsTreeNode(_assetsTree, projectPath, projectPath);
        SortAssetsTree(_assetsTree);
    }
}

bool nsLibraryView::BuildAssetsTreeNode(AssetTreeNode &node,
                                        const nsFilePath &path,
                                        const nsFilePath &projectPath) const {
    static const std::vector<std::string> layoutExtensions = {"layout"};

    nsFilePath::tList items;
    if (!path.Listing(items)) {
        return false;
    }

    bool hasVisibleChildren = false;
    for (const auto &item: items) {
        nsString relativePath = projectPath.GetRelativePath(item);
        if (item.IsFolder()) {
            AssetTreeNode child;
            child.name = item.GetName();
            child.relativePath = relativePath;
            child.fullPath = item;
            child.isFile = false;

            const bool childVisible = BuildAssetsTreeNode(child, item, projectPath);
            node.children.push_back(child);
            hasVisibleChildren = hasVisibleChildren || childVisible;

            continue;
        }

        if (!nsFileUtils::CheckExtension(item, layoutExtensions)) {
            continue;
        }

        AssetTreeNode child;
        child.name = item.GetName();
        child.relativePath = relativePath;
        child.fullPath = item;
        child.isFile = true;
        node.children.push_back(child);
        hasVisibleChildren = true;
    }

    return hasVisibleChildren;
}

void nsLibraryView::SortAssetsTree(AssetTreeNode &node) const {
    std::sort(node.children.begin(), node.children.end(), [](AssetTreeNode &a, AssetTreeNode &b) {
        if (a.isFile != b.isFile) {
            return !a.isFile;
        }
        return strcmp(a.name.AsChar(), b.name.AsChar()) < 0;
    });

    for (auto &child: node.children) {
        SortAssetsTree(child);
    }
}

void nsLibraryView::DrawAssetsTreeNode(AssetTreeNode &node) {
    if (node.isFile) {
        DrawLayoutFileNode(node);
        return;
    }

    ImGui::PushID(node.relativePath.AsChar());
    const bool opened = ImGui::TreeNodeEx(node.name.AsChar(), ImGuiTreeNodeFlags_DefaultOpen);
    if (opened) {
        for (auto &child: node.children) {
            if (IsAssetTreeNodeVisible(child)) {
                DrawAssetsTreeNode(child);
            }
        }
        ImGui::TreePop();
    }
    ImGui::PopID();
}

void nsLibraryView::DrawLayoutFileNode(AssetTreeNode &node) {
    auto &user = _model->project.user;
    const auto path = node.fullPath.AsChar();

    ImGui::PushID(path);
    if (ImGui::Selectable(node.name.AsChar(), user.currentScene == path,
                          ImGuiSelectableFlags_AllowDoubleClick)) {
        if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            Log::Info("Selected: %s", path);
            const std::string value = path;
            const auto undo = nsUndoService::Shared();
            const auto batch = new nsUndoBatch();

            batch->Add(new nsUndoVarChange(user.selectedObject,
                                           static_cast<nsVisualObject2d *>(nullptr)));
            batch->Add(new nsUndoVarChange(user.currentScene, value));

            undo->Push(batch);
        }
    }

    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
        ImGui::SetDragDropPayload("MY_DND_TYPE", path, strlen(path) + 1);
        ImGui::Text("Drop to scene tree...");
        ImGui::EndDragDropSource();
    }
    ImGui::PopID();
}

bool nsLibraryView::IsAssetTreeNodeVisible(AssetTreeNode &node) {
    if (_filter.IsEmpty()) {
        return true;
    }

    if (strstr(node.name.AsChar(), _filter.AsChar())
        || strstr(node.relativePath.AsChar(), _filter.AsChar())) {
        return true;
    }

    if (node.isFile) {
        return false;
    }

    for (auto &child: node.children) {
        if (IsAssetTreeNodeVisible(child)) {
            return true;
        }
    }

    return false;
}
