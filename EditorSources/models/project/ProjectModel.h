// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ProjectModel.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "ParticlesCache.h"
#include "ProjectStateModel.h"
#include "ScenesCache.h"

class nsProjectModel {
public:
    nsScenesCache scenes;
    nsProjectStateModel user;
    nsParticlesCache particles;

    nsProjectModel();

    bool Load(const nsFilePath &projectFolder);
    bool Save(const nsFilePath &projectFolder);
    void Reset();

private:
    std::vector<nsProjectSubModel*> _models;
};
