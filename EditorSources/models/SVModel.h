// Copyright (c) 2003-2024, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file SVModel.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "ProjectStateModel.h"
#include "Core/serialization/var/StringVar.h"
#include "nsLib/locator/Locatable.h"
#include "nsLib/models/Property.h"
#include "project/ProjectModel.h"

class nsSVModel : public nsLocatable, public nsSerializableFile {
public:
    nsSVModel();

    nsStringVar         projectPath;

    nsProperty<float>   zoom;
    nsProperty<int>     blastParticles;

    nsProjectModel      project;
    nsProjectStateModel user;

    nsFilePath GetProjectPath();

    bool Load(const char *fileName) override;
    bool Save(const char *fileName) override;

protected:
    void OnCreated() override;
};
