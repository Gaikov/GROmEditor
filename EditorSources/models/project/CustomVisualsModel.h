// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file CustomVisualsModel.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "ProjectSubModel.h"
#include "Core/serialization/SerializableGroup.h"
#include "Core/serialization/var/ArrayVar.h"
#include "Core/serialization/var/EnumVar.h"
#include "Core/serialization/var/StringVar.h"

class nsCustomVisualProp final : public nsSerializableGroup {
public:
    nsCustomVisualProp();

    enum Type {
        STRING,
        ENUM,
        BOOL,
        NUMBER
    };

    nsStringVar             name = "noname";
    nsEnumVar<Type>         type = STRING;
    nsArrayVar<nsStringVar> enumValues;
};


class nsCustomVisualData final : public nsSerializableGroup {
public:
    nsCustomVisualData();

    nsStringVar                     tag = "notag";
    nsArrayVar<nsCustomVisualProp>  props;
};

class nsCustomVisualsModel final : public nsProjectSubModel {
public:
    nsArrayVar<nsCustomVisualData> visuals;

protected:
    void Reset() override;
    bool Load(const nsFilePath &folder) override;
    bool Save(const nsFilePath &folder) override;
};
