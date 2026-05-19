// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ProjectUtils.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "models/AppModel.h"

class nsProjectUtils final {
public:
    static bool LoadProject(nsAppModel *model, const nsFilePath &path);
};
