//
// Created by Roman on 1/28/2026.
//

#include "CustomObjectPropsView.h"

#include "Core/undo/UndoService.h"
#include "Engine/display/default/DefaultVisualObject.h"
#include "imgui/imgui.h"

bool nsCustomObjectPropsView::IsSupport(nsVisualObject2d *target) {
    return dynamic_cast<nsDefaultVisualObject*>(target);
}

void nsCustomObjectPropsView::Draw(nsVisualObject2d *target) {
    const auto object = dynamic_cast<nsDefaultVisualObject*>(target);

    const auto propsMeta = _model->project.customVisuals.GetMeta(object->GetType());

    if (ImGui::BeginPopup("CustomPropsMenu")) {
        for (int i = 0; i < propsMeta->props.Size(); ++i) {
            const auto propInfo = propsMeta->props.GetItem(i);
            if (!object->customProps.contains(propInfo->name)) {
                if (ImGui::MenuItem(propInfo->name->c_str())) {
                    auto prop =  _factory.Create(propInfo->type);
                    std::string name = propInfo->name;
                    nsUndoService::Shared()->Push(new nsUndoMapInsert(object->customProps, name, prop));
                }
            }
        }

        ImGui::EndPopup();
    }

    if (ImGui::Button("Edit Property")) {
        ImGui::OpenPopup("CustomPropsMenu");
    }
}