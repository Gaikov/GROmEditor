// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file VisualPropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "BaseVisualPropsView.h"
#include "view/components/TextInputUndo.h"

class nsVisualPropsView : public nsBaseVisualPropsView {
public:
    explicit nsVisualPropsView()
        : nsBaseVisualPropsView("Common") {
    }
    void Draw(nsVisualObject2d *target) override;

private:
    nsTextInputUndo<std::string> _idInput = "ID";
};
