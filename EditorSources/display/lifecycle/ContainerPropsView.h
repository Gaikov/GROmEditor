// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ContainerPropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "Core/undo/UndoBatch.h"
#include "Engine/display/undo/UndoAddChild.h"
#include "view/components/BoolInputUndo.h"
#include "view/library/props/BaseVisualPropsView.h"

class nsContainerPropsView : public nsBaseVisualPropsView {
public:
    explicit nsContainerPropsView()
        : nsBaseVisualPropsView("container") {
    }
    bool IsSupport(nsVisualObject2d *target) override;

    bool DrawContextMenu(nsVisualObject2d *target, bool hasPrevItems) override;


protected:
    template<typename TVisual>
    TVisual* MenuItemCreate(const char *name, nsVisualContainer2d *c) const {
        if (ImGui::MenuItem(name)) {
            auto res = _model->project.scenes.Create<TVisual>();
            nsVisualObject2d *obj = res;
            obj->id = name;
            const auto batch = new nsUndoBatch();
            batch->Add(new nsUndoAddChild(c, obj));
            batch->Add(new nsUndoVarChange(_model->user.selectedObject, obj));
            nsUndoService::Shared()->Push(batch);
            return res;
        }
        return nullptr;
    }


    void Draw(nsVisualObject2d *target) override;

private:
    nsBoolInputUndo<bool> _interactiveInput = "Interactive Children";

    IRenState* GetDefaultRenState() const;
};
