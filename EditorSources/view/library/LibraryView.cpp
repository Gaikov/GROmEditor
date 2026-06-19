//
// Created by Roman on 11/18/2025.
//

#include "LibraryView.h"

#include "AssetPolicyRegistry.h"
#include "SceneTreeView.h"
#include "SceneDragDrop.h"
#include "Core/Package.h"
#include "Core/undo/UndoPropertyChange.h"
#include "Core/undo/UndoService.h"
#include "Core/undo/file/UndoFileCreate.h"
#include "Engine/display/VisualObject2d.h"
#include "Engine/display/container/VisualContainer2d.h"
#include "imgui/imgui.h"
#include "nsLib/StrTools.h"
#include "nsLib/locator/ServiceLocator.h"
#include "view/popups/OpenFilePopup.h"
#include "view/popups/PopupsStack.h"
#include <algorithm>
#include <cstring>

nsLibraryView::nsLibraryView() {
    _assetPolicies = Locate<nsAssetPolicyRegistry>();
    _model->settings.projectPath.AddHandler(nsPropChangedName::CHANGED, [&](const nsBaseEvent *) {
        RefreshAssetsTree();
    });
    _model->project.user.currentScene.AddHandler(nsPropChangedName::CHANGED, [&](const nsBaseEvent *) {
        _dragSourcePath = "";
        _dragVisual = nullptr;
        _dragCreationAttempted = false;
    });
    RefreshAssetsTree();
}

void nsLibraryView::Draw() {
    if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        _dragCreationAttempted = false;
    }

    ImGui::Begin("Assets Library");

    ImGui::InputText("Search", _filter.AsChar(), nsString::MAX_SIZE - 1);
    ImGui::SameLine();
    if (ImGui::Button("Clear")) {
        _filter = "";
    }

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
    ImGui::SameLine();
    if (ImGui::Button("Refresh")) {
        RefreshAssetsTree();
    }

    ImGui::End();
}

void nsLibraryView::RefreshAssetsTree() {
    _assetsTree.children.clear();
    _dragSourcePath = "";
    _dragVisual = nullptr;
    _dragCreationAttempted = false;
    const auto projectPath = _model->GetProjectPath();
    if (projectPath.IsFolder()) {
        BuildAssetsTreeNode(_assetsTree, projectPath, projectPath);
        SortAssetsTree(_assetsTree);
    }
}

bool nsLibraryView::BuildAssetsTreeNode(AssetTreeNode &node,
                                        const nsFilePath &path,
                                        const nsFilePath &projectPath) const {
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

        if (!_assetPolicies->HasPolicy(item)) {
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
        DrawAssetFileNode(node);
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

void nsLibraryView::DrawAssetFileNode(AssetTreeNode &node) {
    const auto path = node.fullPath.AsChar();
    const auto policy = _assetPolicies->FindPolicy(node.fullPath);
    if (!policy) {
        return;
    }

    ImGui::PushID(path);
    if (ImGui::Selectable(node.name.AsChar(), policy->IsSelected(node.fullPath),
                          ImGuiSelectableFlags_AllowDoubleClick)) {
        if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            policy->OnDoubleClick(node.fullPath);
        }
    }

    if (policy->CanCreateVisual() && ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
        if (!_dragCreationAttempted) {
            if (!_dragVisual || _dragVisual->GetParent() || !(_dragSourcePath == node.fullPath)) {
                _dragSourcePath = node.fullPath;
                _dragVisual = policy->CreateVisual(node.fullPath);
            }
            _dragCreationAttempted = true;
        }

        if (_dragVisual) {
            ImGui::SetDragDropPayload(DND_TREE_VISUAL_TYPE, &_dragVisual, sizeof(nsVisualObject2d *));
            ImGui::Text("Drop to scene tree...");
        }
        ImGui::EndDragDropSource();
    }
    ImGui::PopID();
}

bool nsLibraryView::IsAssetTreeNodeVisible(AssetTreeNode &node) {
    if (_filter.IsEmpty()) {
        return true;
    }

    if (nsStr::ContainsIgnoreCase(node.name.AsChar(), _filter.AsChar())
        || nsStr::ContainsIgnoreCase(node.relativePath.AsChar(), _filter.AsChar())) {
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
