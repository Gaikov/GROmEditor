// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file MainMenuBar.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "BaseView.h"
#include "SVModel.h"

class nsMainMenuBar : public nsBaseView {
public:
    nsMainMenuBar();
    void Draw() override;

private:
    nsSVModel *_model;
};
