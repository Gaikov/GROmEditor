// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file LibraryView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "view/BaseView.h"

class nsLibraryView : public nsBaseView {
public:
    nsLibraryView();

protected:
    void Draw() override;

    nsString _filter;
};
