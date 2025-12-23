// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file FolderSelectPopup.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "Popup.h"
#include "nsLib/FilePath.h"

class nsOpenFilePopup final : public nsPopup {
public:
    typedef std::function<void(const nsFilePath &)> OpenCallback_t;

    explicit nsOpenFilePopup(const nsFilePath &startPath);
    void SetExtensions(const std::vector<std::string> &extensions);
    void SetOpenCallback(const OpenCallback_t &callback);

private:
    nsFilePath                  _currentPath;
    std::vector<std::string>    _items;
    nsString                    _selectedItem;
    nsFilePath                  _selectedPath = "";
    std::vector<std::string>    _extensions;
    OpenCallback_t              _callback;

protected:
    void DrawContent() override;
    void Refresh();
    void UpdateSelected(const char *item);
    bool ValidatePath() const;
};
