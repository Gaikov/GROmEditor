// Copyright (c) 2003-2024, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file SVModel.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "nsLib/locator/Locatable.h"

class nsSVModel : public nsLocatable {
protected:
    void OnCreated() override;
};