// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file TestView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "BaseView.h"
#include "test/EditablePlotTest.h"


class nsTestView : public nsBaseView {
public:
    nsTestView();
    void Draw() override;

private:
    EditablePlot _editable;
};

