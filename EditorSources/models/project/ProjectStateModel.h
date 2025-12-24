// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file UserState.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "ProjectSubModel.h"
#include "Core/serialization/SerializableFile.h"
#include "Core/serialization/var/BoolVar.h"
#include "Core/serialization/var/ColorVar.h"
#include "Core/serialization/var/FloatVar.h"
#include "Core/serialization/var/StringVar.h"
#include "Engine/display/VisualObject2d.h"

class nsProjectStateModel : public nsSerializableFile, virtual public nsProjectSubModel {
public:
    explicit nsProjectStateModel();

protected:
    void Reset() override;
    bool Load(const nsFilePath &folder) override;
    bool Save(const nsFilePath &folder) override;
    void Validate(nsProjectModel *model) override;

public:
    nsProperty<nsVisualObject2d*> selectedObject = nullptr;

    nsStringVar currentScene = "";
    nsBoolVar xFlip = false;
    nsBoolVar yFlip = false;
    nsFloatVar zoom = 1;
    nsColorVar backColor = nsColor::black;
    nsFloatVar sceneX = 1, sceneY = 1;
    nsBoolVar testView = false;
};
