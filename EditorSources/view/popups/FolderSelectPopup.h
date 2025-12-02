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
    nsFolderSelectDialog();

private:
    nsFilePath _currentPath;
    std::vector<std::string> _folders;
    std::string _selectedFolder;

protected:
    void DrawContent() override;
    void Refresh();
};
