// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ProjectModel.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "ScenesCache.h"
#include "nsLib/models/Property.h"

class nsProjectModel {
public:
    nsProperty<std::string> projectPath;

    nsProperty<std::string> currentScene;
    nsScenesCache scenes;

    nsProjectModel();
};
