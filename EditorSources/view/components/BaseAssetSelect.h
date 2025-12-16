// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file BaseAssetSelect.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "imgui/imgui.h"
#include "models/AppModel.h"
#include "nsLib/FilePath.h"
#include "nsLib/StrTools.h"

class nsBaseAssetSelect : public nsNoCopyable {
public:
    nsBaseAssetSelect();

    void DrawInputField(const char *title, const char *currentPath);
    bool DrawSelectionPopup(const nsString &path);

protected:
    std::vector<std::string> _extensions;
    ImVec2 _popupSize = {100, 100};
    nsAppModel * const _model;

    virtual const char* GetPopupId() = 0;
    virtual void OnClickBrowse() = 0;
    virtual void DrawSelectedInfo() = 0;
    virtual void DrawSelectedPreview() = 0;
    virtual void OnClickSelectPreview(const nsFilePath &path) = 0;

private:
    std::vector<nsFilePath> _files;
    nsString _filter;

    bool HasValidExtension(const nsFilePath &path) const;
};
