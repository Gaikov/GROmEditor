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
#include "Engine/display/property/VisualCustomProp.h"

class nsCustomVisualPropertyMeta final : public nsSerializableGroup {
public:
    nsCustomVisualPropertyMeta();

    nsStringVar                                 name = "noname";
    nsEnumVar<nsVisualCustomProp::Type>         type = nsVisualCustomProp::STRING;
    nsArrayVar<nsStringVar>                     enumValues;
};


class nsCustomVisualMeta final : public nsSerializableGroup {
public:
    nsCustomVisualMeta();

    nsCustomVisualPropertyMeta* GetPropertyMeta(const char *name);

    nsStringVar                     tag = "notag";
    nsArrayVar<nsCustomVisualPropertyMeta>  props;
};

class nsCustomVisualsModel final : public nsProjectSubModel {
public:
    nsArrayVar<nsCustomVisualMeta> visuals;
    nsCustomVisualMeta* GetVisualMeta(const char *tagName);

protected:
    void Reset() override;
    bool Load(const nsFilePath &folder) override;
    bool Save(const nsFilePath &folder) override;
};
