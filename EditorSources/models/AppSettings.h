// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file AppSettings.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "Core/serialization/SerializableFile.h"
#include "Core/serialization/var/StringVar.h"

class nsAppSettings final : public nsSerializableFile {
public:
    nsAppSettings()
        : nsSerializableFile("user"),
          projectPath("project_path", ".") {
        AddItem(&projectPath);
    }

    nsStringVar         projectPath;
};
