// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file UserState.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "ProjectSubModel.h"
#include "Core/serialization/SerializableFile.h"
#include "Core/serialization/SerializableGroup.h"
#include "Core/serialization/var/ArrayVar.h"
#include "Core/serialization/var/BoolVar.h"
#include "Core/serialization/var/ColorVar.h"
#include "Core/serialization/var/FilePathVar.h"
#include "Core/serialization/var/FloatVar.h"
#include "Engine/display/VisualObject2d.h"

class nsSceneState final : public nsSerializableGroup {
public:
    nsSceneState();

    nsFilePathVar scene = "";
    nsFloatVar zoom = 1;
    nsFloatVar x = 1;
    nsFloatVar y = 1;
};

class nsProjectStateModel : public nsSerializableFile, virtual public nsProjectSubModel {
public:
    explicit nsProjectStateModel();
    bool SaveUserState(const nsFilePath &folder);
    void SetBaseFolder(const nsFilePath &folder);
    nsSceneState *FindSceneState(const std::string &scenePath);
    nsSceneState *GetSceneState();

protected:
    void Reset() override;
    bool Load(const nsFilePath &folder) override;
    bool Save(const nsFilePath &folder) override;
    void Validate(nsProjectModel *model) override;

public:
    nsProperty<nsVisualObject2d*> selectedObject = nullptr;

    nsFilePathVar currentScene = "";
    nsArrayVar<nsSceneState> sceneStates;
    nsBoolVar xFlip = false;
    nsBoolVar yFlip = false;
    nsColorVar backColor = nsColor::black;
    nsBoolVar testView = false;
    nsFloatVar backCellSize = 20;
    nsBoolVar showProjectSettings = false;

private:
    nsProjectModel *_projectModel = nullptr;
    nsFilePath _projectFolder = "";
};
