// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file MainMenuBar.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "BaseView.h"
#include "components/menu/MenuBar.h"
#include "models/AppModel.h"

class nsMainMenuBar : public nsBaseView {
public:
    nsMainMenuBar();
    void Draw() override;

private:
    nsMenuBar _menu;
    nsAppModel *_model;

    nsMenuItem *_undo, *_redo;
    nsMenuItem * _xFlip;
    nsMenuItem * _yFlip;
    nsMenuItem * _demoView;
};
