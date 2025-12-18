//
// Created by Roman on 12/17/2025.
//

#include "SpawnerPropsRegistry.h"

#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVectorAdd.h"
#include "Core/undo/UndoVectorRemove.h"
#include "Engine/renderer/particles/spawner/ParticlesAngleSpawner.h"
#include "Engine/renderer/particles/spawner/ParticlesMultiSpawner.h"
#include "imgui/imgui.h"
#include "nsLib/StrTools.h"
#include "view/components/AngleInputUndo.h"

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

    void Draw(nsParticlesSpawner *spawner, nsSpawnerPropsContext *context) override {
        const auto s = dynamic_cast<nsParticlesMultiSpawner*>(spawner);

        if (ImGui::Button("Add")) {
            ImGui::OpenPopup("AddSpawnerPopup");
        }

        if (ImGui::BeginPopup("AddSpawnerPopup")) {
            AddSpawner<nsParticlesAngleSpawner>(s);
            ImGui::EndPopup();
        }

        int buttonId = 0;
        for (auto &child : s->list) {
            ImGui::SeparatorText(child->GetName());

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

    void Draw(nsParticlesSpawner *spawner, nsSpawnerPropsContext *context) override {
        const auto s = dynamic_cast<nsParticlesAngleSpawner*>(spawner);
        _minAngle.Draw(s->minAngle);
        _maxAngle.Draw(s->maxAngle);
    }
};

nsSpawnerPropsRegistry::nsSpawnerPropsRegistry() {
    _views.emplace_back(new nsMultiSpawnerPropsView());
    _views.emplace_back(new nsAngleSpawnerPropsView());
}

void nsSpawnerPropsRegistry::DrawProps(nsParticlesSpawner *spawner) {
    for (auto &v : _views) {
        if (v->IsSupported(spawner)) {
            v->Draw(spawner, this);
        }
    }
}
