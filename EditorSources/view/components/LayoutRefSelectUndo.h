// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file LayoutRefSelectUndo.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "BaseAssetSelect.h"
#include "nsLib/models/Property.h"

class nsLayoutRefSelectUndo : public nsBaseAssetSelect {
public:
    nsLayoutRefSelectUndo();
    void Draw(nsProperty<std::string> &source);

protected:
    const char * GetPopupId() override;
    void OnClickBrowse() override;
    void DrawSelectedInfo() override;
    void DrawSelectedPreview() override;
    void OnClickSelectPreview(const nsFilePath &path) override;

private:
    nsString _passed;
    nsString _selected;
};
