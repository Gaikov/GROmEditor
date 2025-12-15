// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file UserState.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "Core/serialization/SerializableFile.h"
#include "Core/serialization/var/BoolVar.h"
#include "Core/serialization/var/ColorVar.h"
#include "Core/serialization/var/FloatVar.h"
#include "Core/serialization/var/StringVar.h"
#include "Engine/display/VisualObject2d.h"

class nsProjectStateModel : public nsSerializableFile {
public:
    explicit nsProjectStateModel();

    nsProperty<nsVisualObject2d*> selectedObject = nullptr;

    nsStringVar currentScene;
    nsBoolVar xFlip;
    nsBoolVar yFlip;
    nsFloatVar zoom;
    nsColorVar backColor;
    nsFloatVar sceneX, sceneY;
    nsBoolVar testView;
};
