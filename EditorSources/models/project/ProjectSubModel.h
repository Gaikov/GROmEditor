// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ProjectSubModel.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "nsLib/FilePath.h"
#include "nsLib/utils/NoCopyable.h"

class nsProjectModel;

class nsProjectSubModel : public nsNoCopyable {
    friend class nsProjectModel;

protected:
    virtual void Reset() = 0;

    virtual bool Load(const nsFilePath &folder) = 0;
    virtual bool Save(const nsFilePath &folder) = 0;
    virtual void Validate(nsProjectModel *model) {};
};
