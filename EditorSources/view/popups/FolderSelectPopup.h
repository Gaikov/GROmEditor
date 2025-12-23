// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file FolderSelectPopup.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "Popup.h"
#include "nsLib/FilePath.h"

class nsFolderSelectDialog final : public nsPopup {
public:
    explicit nsFolderSelectDialog(const nsFilePath &startPath);
    void SetExtensions(const std::vector<std::string> &extensions);

private:
    nsFilePath _currentPath;
    std::vector<std::string> _items;
    nsString _selectedItem;
    nsFilePath _selectedPath = "";
    std::vector<std::string> _extensions;

protected:
    void DrawContent() override;
    void Refresh();
    void UpdateSelected(const char *item);
};
