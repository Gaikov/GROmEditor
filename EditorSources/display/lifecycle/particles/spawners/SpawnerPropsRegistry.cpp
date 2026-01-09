//
// Created by Roman on 12/17/2025.
//

#include "SpawnerPropsRegistry.h"

#include "Core/undo/UndoBatch.h"
#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVectorAdd.h"
#include "Core/undo/UndoVectorRemove.h"
#include "Core/undo/array/UndoArrayAdd.h"
#include "Engine/renderer/particles/spawner/ParticlesAngleSpawner.h"
#include "Engine/renderer/particles/spawner/ParticlesColorSpawner.h"
#include "Engine/renderer/particles/spawner/ParticlesMultiSpawner.h"
#include "Engine/renderer/particles/spawner/position/ParticlesCircleSpawner.h"
#include "Engine/renderer/particles/spawner/position/ParticlesEdgesSpawner.h"
#include "imgui/imgui.h"
#include "nsLib/StrTools.h"
#include "view/components/AngleInputUndo.h"
#include "view/components/BoolInputUndo.h"
#include "view/components/ColorInputUndo.h"
#include "view/components/FloatInputUndo.h"
#include "view/components/Vec2InputUndo.h"

class nsMultiSpawnerPropsView : public nsSpawnerPropsView {
protected:
    bool IsSupported(nsParticlesSpawner *spawner) override {
        return dynamic_cast<nsParticlesMultiSpawner*>(spawner);
    }

    template<typename TSpawner>
    void AddSpawner(nsParticlesMultiSpawner *spawner) {
        if (ImGui::MenuItem(TSpawner::TITLE)) {
            const nsParticlesSpawner::sp_t s = new TSpawner();
            nsUndoService::Shared()->Push(new nsUndoVectorAdd(spawner->list, s));
        }
    }

    void Draw(nsParticlesSpawner *spawner, nsPropsContext *context) override {
        const auto s = dynamic_cast<nsParticlesMultiSpawner*>(spawner);

        if (ImGui::Button("Add")) {
            ImGui::OpenPopup("AddSpawnerPopup");
        }

        if (ImGui::BeginPopup("AddSpawnerPopup")) {
            AddSpawner<nsParticlesAngleSpawner>(s);
            AddSpawner<nsParticlesCircleSpawner>(s);
            AddSpawner<nsParticlesColorSpawner>(s);
            AddSpawner<nsParticlesEdgesSpawner>(s);
            ImGui::EndPopup();
        }

        int buttonId = 0;
        for (auto &child : s->list) {
            ImGui::SeparatorText(child->GetTitle());

            nsString buttonLabel;
            buttonLabel.Format("Remove##%d", buttonId++);
            if (ImGui::Button(buttonLabel)) {
                nsUndoService::Shared()->Push(new nsUndoVectorRemove(s->list, child));
                break;
            }

            context->DrawProps(child);
        }
    }
};

class nsAngleSpawnerPropsView final : public nsSpawnerPropsView {
protected:
    nsAngleInputUndo<float> _minAngle = "Min Angle";
    nsAngleInputUndo<float> _maxAngle = "Max Angle";

    bool IsSupported(nsParticlesSpawner *spawner) override {
        return dynamic_cast<nsParticlesAngleSpawner*>(spawner);
    }

    void Draw(nsParticlesSpawner *spawner, nsPropsContext *context) override {
        const auto s = dynamic_cast<nsParticlesAngleSpawner*>(spawner);
        _minAngle.Draw(s->minAngle);
        _maxAngle.Draw(s->maxAngle);
    }
};

class nsCircleSpawnerPropsView final : public nsSpawnerPropsView {
protected:

    nsFloatInputUndo<float> _radius = "Radius";
    nsBoolInputUndo<bool> _onEdge = "On Edge";
    nsFloatInputUndo<float> _minRadius = "Min Radius";

    bool IsSupported(nsParticlesSpawner *spawner) override {
        return dynamic_cast<nsParticlesCircleSpawner*>(spawner);
    }

    void Draw(nsParticlesSpawner *spawner, nsPropsContext *context) override {
        const auto s = dynamic_cast<nsParticlesCircleSpawner*>(spawner);
        _radius.Draw(s->radius);
        _onEdge.Draw(s->onEdge);
        if (s->onEdge) {
            _minRadius.Draw(s->minRadius);
        }
    }
};

class nsColorSpawnerPropsView final : public nsSpawnerPropsView {
protected:
    nsColorInputUndo<nsColor> _color1 = "Color 1";
    nsColorInputUndo<nsColor> _color2 = "Color 2";

    bool IsSupported(nsParticlesSpawner *spawner) override {
        return dynamic_cast<nsParticlesColorSpawner*>(spawner);
    }
    void Draw(nsParticlesSpawner *spawner, nsPropsContext *context) override {
        const auto s = dynamic_cast<nsParticlesColorSpawner*>(spawner);
        _color1.Draw(s->color1);
        _color2.Draw(s->color2);
    }
};

class nsEdgesSpawnerPropsView final : public nsSpawnerPropsView {
protected:
    bool IsSupported(nsParticlesSpawner *spawner) override {
        return dynamic_cast<nsParticlesEdgesSpawner*>(spawner);
    }
    void Draw(nsParticlesSpawner *spawner, nsPropsContext *context) override {
        const auto s = dynamic_cast<nsParticlesEdgesSpawner*>(spawner);
        if (ImGui::Button("Add Edge")) {
            nsUndoBatch batch;
            batch.Add(new nsUndoArrayAdd(s->points, std::make_shared<nsPoint>(nsVec2())));
            batch.Add(new nsUndoArrayAdd(s->points, std::make_shared<nsPoint>(nsVec2())));
            nsUndoService::Shared()->Push(&batch);
        }

        nsString title;
        const auto &items = s->points.GetItems();
        const int numEdges = items.size() / 2;
        for (int i = 0; i < numEdges; ++i) {
            const auto &p1 = items[i * 2];
            const auto &p2 = items[i * 2 + 1];

            ImGui::Separator();
            title.Format("Remove##point%d", i);
            if (ImGui::Button(title)) {

            } else {
                title.Format("Point 1##%d", i);
                nsVec2InputUndo<nsPoint>::DrawField( title, *p1.get());

                title.Format("Point 2##%d", i);
                nsVec2InputUndo<nsPoint>::DrawField( title, *p2.get());
            }
        }
    }
};

nsSpawnerPropsRegistry::nsSpawnerPropsRegistry() {
    _views.emplace_back(new nsMultiSpawnerPropsView());
    _views.emplace_back(new nsAngleSpawnerPropsView());
    _views.emplace_back(new nsCircleSpawnerPropsView());
    _views.emplace_back(new nsColorSpawnerPropsView());
    _views.emplace_back(new nsEdgesSpawnerPropsView());
}
