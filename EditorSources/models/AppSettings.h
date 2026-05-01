// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file AppSettings.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "Core/serialization/SerializableFile.h"
#include "Core/serialization/var/StringVar.h"
#include "Core/serialization/var/FloatVar.h"

class nsAppSettings final : public nsSerializableFile {
public:
    nsAppSettings() {
        AddItem("project_path", &projectPath);
        AddItem("font_size", &fontSize);
    }

    nsStringVar         projectPath = ".";
    nsFloatVar          fontSize = 16.0f;
};
