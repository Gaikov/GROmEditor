//
// Created by Roman on 1/26/2026.
//

#include "ProjectSettingsPopup.h"

#include "Core/undo/array/UndoArrayAdd.h"
#include "Core/undo/array/UndoArrayRemove.h"
#include "nsLib/locator/ServiceLocator.h"
#include "view/components/TextInputUndo.h"
#include "view/popups/PopupsStack.h"

nsProjectSettingsPopup::nsProjectSettingsPopup() {
    _model->project.user.showProjectSettings.AddHandler(nsPropChangedName::CHANGED, [&](const nsBaseEvent *) {
        visible = _model->project.user.showProjectSettings;
    });
    visible = _model->project.user.showProjectSettings;
    _typeInput.AddVariant("String", nsCustomVisualProp::STRING);
    _typeInput.AddVariant("Enum", nsCustomVisualProp::ENUM);
    _typeInput.AddVariant("Bool", nsCustomVisualProp::BOOL);
    _typeInput.AddVariant("Number", nsCustomVisualProp::NUMBER);
}

void nsProjectSettingsPopup::Draw() {
    bool visible = true;

    if (ImGui::Begin("Project Settings", &visible, ImGuiWindowFlags_NoCollapse)) {
        auto &project = Locate<nsAppModel>()->project;
        if (ImGui::CollapsingHeader("Custom Visuals", ImGuiTreeNodeFlags_DefaultOpen)) {
            auto &visuals = project.customVisuals.visuals;
            if (ImGui::Button("Add Object")) {
                const std::shared_ptr<nsSerializable> data = std::make_shared<nsCustomVisualData>();
                nsUndoService::Shared()->Push(new nsUndoArrayAdd(visuals, data));
            }

            nsString title;
            auto &items = visuals.GetItems();
            for (int i = 0; i < items.size(); ++i) {
                auto &v = items[i];
                const auto obj = dynamic_cast<nsCustomVisualData *>(v.get());

                title.Format("%s##%d", obj->tag->c_str(), i);
                if (ImGui::CollapsingHeader(title, ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::BeginChild(title, ImVec2(0, 0),
                                      ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Borders,
                                      ImGuiWindowFlags_HorizontalScrollbar);

                    title.Format("Tag##%d", i);
                    nsTextInputUndo<nsStringVar>::DrawField(title, obj->tag);

                    ImGui::SameLine();
                    title.Format("Remove##%d", i);
                    if (ImGui::Button(title)) {
                        nsUndoService::Shared()->Push(new nsUndoArrayRemove(visuals, v));
                    }

                    DrawObjectProps(obj, i);

                    ImGui::Separator();
                    title.Format("Add Property##%d", i);
                    if (ImGui::Button(title)) {
                        const std::shared_ptr<nsSerializable> data = std::make_shared<nsCustomVisualProp>();
                        nsUndoService::Shared()->Push(new nsUndoArrayAdd(obj->props, data));
                    }

                    ImGui::EndChild();
                }
            }
        }
        ImGui::End();
    }

    if (!visible) {
        nsUndoService::Shared()->Push(new nsUndoVarChange(_model->project.user.showProjectSettings, false));
    }
}

void nsProjectSettingsPopup::DrawObjectProps(nsCustomVisualData *data, int index) {
    nsString title;

    auto &props = data->props.GetItems();
    for (int i = 0; i < props.size(); ++i) {
        const auto prop = dynamic_cast<nsCustomVisualProp *>(props[i].get());
        ImGui::Separator();
        title.Format("Prop Name##%d%d", index, i);
        nsTextInputUndo<nsStringVar>::DrawField(title, prop->name);

        ImGui::SameLine();
        title.Format("Remove##%d%d", index, i);
        if (ImGui::Button(title)) {
            nsUndoService::Shared()->Push(new nsUndoArrayRemove(data->props, props[i]));
        }

        title.Format("Type##%d%d", index, i);
        _typeInput.Draw(prop->type, title);

        if (prop->type == nsCustomVisualProp::ENUM) {
            title.Format("Add Variant##%d%d", index, i);

            if (ImGui::Button(title)) {
                const std::shared_ptr<nsSerializable> enumValue = std::make_shared<nsStringVar>();
                nsUndoService::Shared()->Push(new nsUndoArrayAdd(prop->enumValues, enumValue));
            }

            auto &values = prop->enumValues;
            for (int j = 0; j < values.Size(); ++j) {
                title.Format("Variant##%d%d%d", index, i, j);
                const auto value = dynamic_cast<nsStringVar *>(values[j].get());
                nsTextInputUndo<nsStringVar>::DrawField(title, *value);
            }
        }
    }
}
