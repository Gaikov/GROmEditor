// Copyright (c) 2003-2024, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file SVModel.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "AppSettings.h"
#include "nsLib/locator/Locatable.h"
#include "nsLib/models/Property.h"
#include "project/ProjectModel.h"

class nsAppModel : public nsLocatable {
public:
    nsAppModel();

    nsAppSettings       settings;

    nsProperty<int>     blastParticles;

    nsProjectModel      project;

    [[nodiscard]] nsFilePath GetProjectPath() const;

    bool Load();
    bool Save();

protected:
    void OnCreated() override;
};
