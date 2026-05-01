// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file LibraryView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "nsLib/FilePath.h"
#include "view/BaseView.h"
#include <vector>

class nsLibraryView : public nsBaseView {
public:
    nsLibraryView();

protected:
    void Draw() override;

private:
    struct AssetTreeNode {
        nsString name;
        nsString relativePath;
        nsFilePath fullPath = "";
        bool isFile = false;
        std::vector<AssetTreeNode> children;
    };

    void RefreshAssetsTree();
    bool BuildAssetsTreeNode(AssetTreeNode &node, const nsFilePath &path, const nsFilePath &projectPath) const;
    void SortAssetsTree(AssetTreeNode &node) const;
    void DrawAssetsTreeNode(AssetTreeNode &node);
    void DrawLayoutFileNode(AssetTreeNode &node);
    bool IsAssetTreeNodeVisible(AssetTreeNode &node);

protected:
    nsString _filter;
    AssetTreeNode _assetsTree;
};
