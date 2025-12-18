//
// Created by Roman on 12/17/2025.
//

#include "SpawnerPropsRegistry.h"

#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVectorRemove.h"
#include "Engine/renderer/particles/spawner/ParticlesMultiSpawner.h"
#include "imgui/imgui.h"
#include "nsLib/StrTools.h"

class nsMultiSpawnerPropsView : public nsSpawnerPropsView {
protected:
    bool IsSupported(nsParticlesSpawner *spawner) override {
        return dynamic_cast<nsParticlesMultiSpawner*>(spawner);
    }

    void Draw(nsParticlesSpawner *spawner, nsSpawnerPropsContext *context) override {
        const auto s = dynamic_cast<nsParticlesMultiSpawner*>(spawner);

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

nsSpawnerPropsRegistry::nsSpawnerPropsRegistry() {
    _views.emplace_back(new nsMultiSpawnerPropsView());
}

void nsSpawnerPropsRegistry::DrawProps(nsParticlesSpawner *spawner) {
    for (auto &v : _views) {
        if (v->IsSupported(spawner)) {
            v->Draw(spawner, this);
        }
    }
}
