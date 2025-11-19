// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file UserState.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "Core/serialization/SerializableFile.h"
#include "Core/serialization/var/BoolVar.h"
#include "Core/serialization/var/StringVar.h"

class nsProjectStateModel : public nsSerializableFile {
public:
    explicit nsProjectStateModel();

    nsStringVar currentScene;
    nsBoolVar xFlip;
    nsBoolVar yFlip;
    nsBoolVar emitParticles;
    nsBoolVar testView;
};
