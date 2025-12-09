// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ProjectModel.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "ScenesCache.h"

class nsProjectModel {
public:
    nsScenesCache scenes;

    nsProjectModel() = default;

    bool Load(const nsFilePath &projectFolder);
    bool Save(const nsFilePath &projectFolder) const;
};
