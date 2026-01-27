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
    void DrawObjectProps(nsCustomVisualMeta *data, int index);

private:
    nsEnumInputUndo<nsCustomVisualPropertyMeta::Type, nsEnumVar<nsCustomVisualPropertyMeta::Type>> _typeInput = "Type";
};

