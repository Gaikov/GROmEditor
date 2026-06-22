// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file AppSettings.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "Core/serialization/SerializableFile.h"
#include "Core/serialization/var/StringVar.h"
#include "Core/serialization/var/FloatVar.h"
#include "Core/serialization/var/ColorVar.h"

class nsAppSettings final : public nsSerializableFile {
public:
    nsAppSettings() {
        AddItem("project_path", &projectPath);
        AddItem("font_size", &fontSize);
        AddItem("selection_color", &selectionColor);
        AddItem("origin_color", &originColor);
        AddItem("hover_color", &hoverColor);
    }

    nsStringVar         projectPath = ".";
    nsFloatVar          fontSize = 16.0f;
    nsColorVar          selectionColor = nsColor::green;
    nsColorVar          originColor = nsColor::magenta;
    nsColorVar          hoverColor = nsColor(0.3f, 0.5f, 1.0f, 0.7f);
};
