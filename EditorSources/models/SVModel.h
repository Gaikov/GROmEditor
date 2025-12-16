// Copyright (c) 2003-2024, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file SVModel.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "AppSettings.h"
#include "ProjectStateModel.h"
#include "nsLib/locator/Locatable.h"
#include "nsLib/models/Property.h"
#include "project/ProjectModel.h"

class nsSVModel : public nsLocatable {
public:
    nsSVModel();

    nsAppSettings       settings;

    nsProperty<int>     blastParticles;

    nsProjectModel      project;
    nsProjectStateModel user;

    nsFilePath GetProjectPath() const;

    bool Load();
    bool Save();

protected:
    void OnCreated() override;
};
