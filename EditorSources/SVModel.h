// Copyright (c) 2003-2024, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file SVModel.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "nsLib/locator/Locatable.h"
#include "nsLib/models/Property.h"

class nsSVModel : public nsLocatable {
public:
    nsSVModel();

    nsProperty<float>   zoom;
    nsProperty<bool>    xFlip;
    nsProperty<bool>    yFlip;
    nsProperty<bool>    emitParticles;

protected:
    void OnCreated() override;
};