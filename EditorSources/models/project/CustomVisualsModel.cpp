//
// Created by Roman on 1/26/2026.
//

#include "CustomVisualsModel.h"

#include "Core/ParseFile.h"
#include "Core/serialization/SerializeUtils.h"

#define FILE_NAME "custom_visuals.ggml"
#define PROP_NAME "custom_visual"

nsCustomVisualProp::nsCustomVisualProp() {
    AddItem("name", &name);
    AddItem("type", &type);
    AddItem("enum_values", &enumValues);
}

nsCustomVisualData::nsCustomVisualData() {
    AddItem("tag", &tag);
    AddItem("props", &props);
}

void nsCustomVisualsModel::Reset() {
    visuals.Clear();
}

bool nsCustomVisualsModel::Load(const nsFilePath &folder) {
    nsParseFile pf;
    const auto ss = pf.BeginFile(folder.ResolvePath(FILE_NAME));
    if (!ss) {
        return true;
    }

    nsSerializeUtils::DeserializeVar(ss, PROP_NAME, &visuals);
    return true;
}

bool nsCustomVisualsModel::Save(const nsFilePath &folder) {
    nsScriptSaver saver(folder.ResolvePath(FILE_NAME));
    if (saver.IsValid()) {
        nsSerializeUtils::SerializeVar(saver, PROP_NAME, &visuals);
    }
    return true;
}
