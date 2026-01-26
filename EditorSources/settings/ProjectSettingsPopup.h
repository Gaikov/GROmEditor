// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ProjectSettingsPopup.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "view/BaseView.h"

class nsProjectSettingsPopup : public nsBaseView {
public:
    nsProjectSettingsPopup();

protected:
    void Draw() override;
};

