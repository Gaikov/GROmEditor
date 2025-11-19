// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file VisualPropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "BaseVisualPropsView.h"
#include "view/components/UndoTextInput.h"

class nsVisualPropsView : public nsBaseVisualPropsView {
public:
    void Draw(nsVisualObject2d *target) override;

private:
    nsUndoTextInput<std::string> _idInput = "ID";
};
