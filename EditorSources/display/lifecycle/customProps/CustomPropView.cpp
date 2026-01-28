//
// Created by Roman on 1/28/2026.
//

#include "CustomPropView.h"
#include "Core/serialization/var/BoolVar.h"
#include "Core/serialization/var/FloatVar.h"
#include "view/components/BoolInputUndo.h"
#include "view/components/FloatInputUndo.h"
#include "view/components/TextInputUndo.h"

void nsCustomBoolView::DrawProperty(const char *name, nsVisualCustomProp *prop, nsCustomVisualPropertyMeta *meta) {
    const auto value = dynamic_cast<nsBoolVar*>(prop->GetValue());
    nsBoolInputUndo<nsBoolVar>::DrawField(name, *value);
}

void nsCustomEnumView::DrawProperty(const char *name, nsVisualCustomProp *prop, nsCustomVisualPropertyMeta *meta) {
    if (prop != _lastProp) {
        _lastProp = prop;
        _inputEnum.ClearVariants();
        for (int i = 0; i < meta->enumValues.Size(); ++i) {
            const auto item = meta->enumValues.GetItem(i);
            const auto value = item->GetValue().c_str();
            _inputEnum.AddVariant(value, value);
        }
    }

    const auto value = dynamic_cast<nsStringVar*>(prop->GetValue());

    _inputEnum.Draw(*value, name);
}

void nsCustomFloatView::DrawProperty(const char *name, nsVisualCustomProp *prop, nsCustomVisualPropertyMeta *meta) {
    const auto value = dynamic_cast<nsFloatVar*>(prop->GetValue());
    nsFloatInputUndo<nsFloatVar>::DrawField(name, *value);
}

void nsCustomStringView::DrawProperty(const char *name, nsVisualCustomProp *prop, nsCustomVisualPropertyMeta *meta) {
    const auto value = dynamic_cast<nsStringVar*>(prop->GetValue());
    nsTextInputUndo<nsStringVar>::DrawField(name, *value);
}
