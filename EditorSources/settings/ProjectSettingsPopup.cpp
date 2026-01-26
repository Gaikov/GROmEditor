//
// Created by Roman on 1/26/2026.
//

#include "ProjectSettingsPopup.h"

#include "Core/undo/array/UndoArrayAdd.h"
#include "nsLib/locator/ServiceLocator.h"
#include "view/components/TextInputUndo.h"
#include "view/popups/PopupsStack.h"

nsProjectSettingsPopup::nsProjectSettingsPopup() {
    _model->project.user.showProjectSettings.AddHandler(nsPropChangedName::CHANGED, [&](const nsBaseEvent *) {
        visible = _model->project.user.showProjectSettings;
    });
    visible = _model->project.user.showProjectSettings;
}

void nsProjectSettingsPopup::Draw() {
    bool visible = true;
    ImGui::SetNextWindowFocus();
    if (ImGui::Begin("Project Settings", &visible, ImGuiWindowFlags_NoCollapse)) {
        auto &project = Locate<nsAppModel>()->project;
        if (ImGui::CollapsingHeader("Custom Visuals", ImGuiTreeNodeFlags_DefaultOpen)) {
            auto &visuals = project.customVisuals.visuals;
            if (ImGui::Button("Add Object")) {
                const auto data = std::make_shared<nsCustomVisualData>();
                nsUndoService::Shared()->Push(new nsUndoArrayAdd(visuals, data));
            }

            ImGui::Separator();
            auto &items = visuals.GetItems();
            for (int i = 0; i < items.size(); ++i) {
                auto &v = items[i];
                const auto obj = dynamic_cast<nsCustomVisualData*>(v.get());
                ImGui::SeparatorText(obj->tag->c_str());

                nsString title;
                title.Format("Tag##%d", i);
                nsTextInputUndo<nsStringVar>::DrawField(title, obj->tag);

                title.Format("Remove##%d", i);

                ImGui::Separator();
            }
        }
        ImGui::End();
    }

    if (!visible) {
        nsUndoService::Shared()->Push(new nsUndoVarChange(_model->project.user.showProjectSettings, false));
    }
}