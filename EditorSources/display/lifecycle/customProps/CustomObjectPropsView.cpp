//
// Created by Roman on 1/28/2026.
//

#include "CustomObjectPropsView.h"

#include "Core/undo/UndoService.h"
#include "Engine/display/default/DefaultVisualObject.h"
#include "imgui/imgui.h"

#define CUSTOM_OBJECT_POPUP_ID "Custom Object Props"

nsCustomObjectPropsView::nsCustomObjectPropsView() : nsBaseVisualPropsView("Custom Properties") {
    _views[nsVisualCustomProp::BOOL] = std::make_shared<nsCustomBoolView>();
    _views[nsVisualCustomProp::ENUM] = std::make_shared<nsCustomEnumView>();
}

bool nsCustomObjectPropsView::IsSupport(nsVisualObject2d *target) {
    return dynamic_cast<nsDefaultVisualObject *>(target);
}

void nsCustomObjectPropsView::Draw(nsVisualObject2d *target) {
    const auto object = dynamic_cast<nsDefaultVisualObject *>(target);

    const auto propsMeta = _model->project.customVisuals.GetVisualMeta(object->GetType());

    if (ImGui::BeginPopup(CUSTOM_OBJECT_POPUP_ID)) {
        for (int i = 0; i < propsMeta->props.Size(); ++i) {
            const auto propInfo = propsMeta->props.GetItem(i);
            if (!object->customProps.contains(propInfo->name)) {
                if (ImGui::MenuItem(propInfo->name->c_str())) {
                    auto prop = _factory.Create(propInfo->type);
                    std::string name = propInfo->name;
                    nsUndoService::Shared()->Push(new nsUndoMapInsert(object->customProps, name, prop));
                }
            }
        }

        ImGui::EndPopup();
    }

    if (ImGui::Button("Edit Property")) {
        ImGui::OpenPopup(CUSTOM_OBJECT_POPUP_ID);
    }

    for (auto &[name, prop]: object->customProps) {
        const auto propMeta = propsMeta->GetPropertyMeta(name.c_str());
        if (!propMeta) {
            Log::Warning("Custom property '%s' not found in meta", name.c_str());
        } else {
            DrawProperty(name.c_str(), prop.get(), propMeta);
        }
    }
}

void nsCustomObjectPropsView::DrawProperty(const char *name, nsVisualCustomProp *prop,
                                           nsCustomVisualPropertyMeta *meta) {
    const auto type = prop->GetType();
    if (type != meta->type) {
        Log::Warning("Custom visual property '%s' has wrong type!", name);
    }

    if (const auto view = _views.find(type); view != _views.end()) {
        view->second->DrawProperty(name, prop, meta);
    } else {
        ImGui::Text("Not implemented %s", name);
    }
}