// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file BaseAssetSelect.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "nsLib/FilePath.h"
#include "nsLib/StrTools.h"

class nsBaseAssetSelect {
public:
    virtual ~nsBaseAssetSelect() = default;

    void DrawInputField(const char *title, const char *currentPath);

protected:
    std::vector<std::string> _extensions;

    virtual const char* GetPopupId() = 0;
    virtual void OnClickBrowse() = 0;

    std::vector<nsFilePath> _files;

private:
    bool HasValidExtension(const nsFilePath &path) const;
};
