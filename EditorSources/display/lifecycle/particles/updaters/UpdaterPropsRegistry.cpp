//
// Created by Roman on 1/6/2026.
//

#include "UpdaterPropsRegistry.h"

#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVectorAdd.h"
#include "Core/undo/UndoVectorRemove.h"
#include "Engine/renderer/particles/updater/ParticlesMultiUpdater.h"
#include "Engine/renderer/particles/updater/ParticlesSizeUpdater.h"
#include "imgui/imgui.h"
#include "nsLib/StrTools.h"
#include "view/components/FloatInputUndo.h"

class nsMultiUpdaterPropsView : public nsUpdaterPropsView {
public:

    template<typename TUpdater>
    void AddSpawner(nsParticlesCompositeUpdater *updater) {
        if (ImGui::MenuItem(TUpdater::TITLE)) {
            const nsParticlesUpdater::sp_t s = new TUpdater();
            nsUndoService::Shared()->Push(new nsUndoVectorAdd(updater->list, s));
        }
    }

    bool IsSupported(nsParticlesUpdater *object) override {
        return dynamic_cast<nsParticlesCompositeUpdater*>(object);
    }

    void Draw(nsParticlesUpdater *object, nsPropsContext *context) override {
        const auto u = dynamic_cast<nsParticlesCompositeUpdater*>(object);

        if (ImGui::Button("Add")) {
            ImGui::OpenPopup(ADD_UPDATER_POPUP_ID);
        }

        if (ImGui::BeginPopup(ADD_UPDATER_POPUP_ID)) {
            AddSpawner<nsParticlesSizeUpdater>(u);
            ImGui::EndPopup();
        }

        int buttonId = 0;
        for (auto &child : u->list) {
            ImGui::SeparatorText(child->GetTitle());

            nsString buttonLabel;
            buttonLabel.Format("Remove##%d", buttonId++);
            if (ImGui::Button(buttonLabel)) {
                nsUndoService::Shared()->Push(new nsUndoVectorRemove(u->list, child));
                break;
            }

            context->DrawProps(child);
        }
    }

private:
    static constexpr auto ADD_UPDATER_POPUP_ID = "AddUpdaterPopup";
};

class nsSizeUpdaterPropsView : public nsUpdaterPropsView {
public:
    bool IsSupported(nsParticlesUpdater *object) override {
        return dynamic_cast<nsParticlesSizeUpdater*>(object);
    }

    void Draw(nsParticlesUpdater *object, nsPropsContext *context) override {
        const auto s = dynamic_cast<nsParticlesSizeUpdater*>(object);
        _size.Draw(s->scale);
    }
    nsFloatInputUndo<float> _size = "Scale To";
};

nsUpdaterPropsRegistry::nsUpdaterPropsRegistry() {
    _views.emplace_back(new nsMultiUpdaterPropsView());
    _views.emplace_back(new nsSizeUpdaterPropsView());
}