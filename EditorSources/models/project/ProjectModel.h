// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ProjectModel.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "ScenesCache.h"
#include "Core/serialization/SerializableFile.h"
#include "Core/serialization/var/StringVar.h"
#include "nsLib/models/Property.h"

class nsProjectModel : public nsSerializableFile {
public:
    nsProperty<std::string> projectPath;

    nsStringVar currentScene;
    nsScenesCache scenes;

    nsProjectModel();
};
