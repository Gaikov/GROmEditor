// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ProjectSettingsPopup.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "view/BaseView.h"
#include "view/components/EnumInputUndo.h"

class nsProjectSettingsPopup : public nsBaseView {
public:
    nsProjectSettingsPopup();

protected:
    void Draw() override;
    void DrawObjectProps(nsCustomVisualData *data, int index);

private:
    nsEnumInputUndo<nsCustomVisualProp::Type, nsEnumVar<nsCustomVisualProp::Type>> _typeInput = "Type";
};

