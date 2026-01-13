//
// Created by Roman on 1/6/2026.
//

#include "UpdaterPropsRegistry.h"

#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVectorAdd.h"
#include "Core/undo/UndoVectorRemove.h"
#include "Engine/renderer/particles/updater/ParticlesGravityUpdater.h"
#include "Engine/renderer/particles/updater/ParticlesMultiUpdater.h"
#include "Engine/renderer/particles/updater/ParticlesSizeUpdater.h"
#include "Engine/renderer/particles/updater/ParticlesVelocityApplyUpdater.h"
#include "Engine/renderer/particles/updater/ParticlesVelToAngleUpdater.h"
#include "Engine/renderer/particles/updater/velocity/ParticlesVelDampUpdater.h"
#include "imgui/imgui.h"
#include "nsLib/StrTools.h"
#include "view/components/FloatInputUndo.h"
#include "view/components/Vec2InputUndo.h"

class nsMultiUpdaterPropsView : public nsUpdaterPropsView {
public:

    template<typename TUpdater>
    void AddUpdater(nsParticlesCompositeUpdater *updater) {
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
            AddUpdater<nsParticlesSizeUpdater>(u);
            AddUpdater<nsParticlesGravityUpdater>(u);
            AddUpdater<nsParticlesVelDampUpdater>(u);
            AddUpdater<nsParticlesVelocityApplyUpdater>(u);
            AddUpdater<nsParticlesVelToAngleUpdater>(u);
            ImGui::EndPopup();
        }

        int buttonId = 0;
        for (auto &child : u->list) {
            ImGui::SeparatorText(child->GetTitle());

            context->DrawProps(child);

            nsString buttonLabel;
            buttonLabel.Format("Remove##%d", buttonId++);
            if (ImGui::Button(buttonLabel)) {
                nsUndoService::Shared()->Push(new nsUndoVectorRemove(u->list, child));
                break;
            }
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

class nsGravityUpdaterPropsView : public nsUpdaterPropsView {
public:
    bool IsSupported(nsParticlesUpdater *object) override {
        return dynamic_cast<nsParticlesGravityUpdater*>(object);
    }

    void Draw(nsParticlesUpdater *object, nsPropsContext *context) override {
        const auto g = dynamic_cast<nsParticlesGravityUpdater*>(object);
        _gravity.Draw(g->gravity);
    }

    nsVec2InputUndo<nsVec2> _gravity = "Gravity";
};

class nsVelDampUpdaterPropsView : public nsUpdaterPropsView {
public:
    bool IsSupported(nsParticlesUpdater *object) override {
        return dynamic_cast<nsParticlesVelDampUpdater*>(object);
    }

    void Draw(nsParticlesUpdater *object, nsPropsContext *context) override {
        const auto v = dynamic_cast<nsParticlesVelDampUpdater*>(object);
        _damping.Draw(v->value);
    }

    nsFloatInputUndo<float> _damping = "Damping";
};

nsUpdaterPropsRegistry::nsUpdaterPropsRegistry() {
    _views.emplace_back(new nsMultiUpdaterPropsView());
    _views.emplace_back(new nsSizeUpdaterPropsView());
    _views.emplace_back(new nsGravityUpdaterPropsView());
    _views.emplace_back(new nsVelDampUpdaterPropsView());
}