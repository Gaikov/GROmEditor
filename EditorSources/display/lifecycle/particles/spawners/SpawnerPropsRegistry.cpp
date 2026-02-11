//
// Created by Roman on 12/17/2025.
//

#include "SpawnerPropsRegistry.h"

#include "Core/undo/UndoBatch.h"
#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVectorAdd.h"
#include "Core/undo/UndoVectorRemove.h"
#include "Core/undo/array/UndoArrayAdd.h"
#include "Core/undo/array/UndoArrayRemove.h"
#include "Engine/renderer/particles/spawner/ParticlesAngleSpawner.h"
#include "Engine/renderer/particles/spawner/ParticlesColorSpawner.h"
#include "Engine/renderer/particles/spawner/ParticlesLifeSpawner.h"
#include "Engine/renderer/particles/spawner/ParticlesMultiSpawner.h"
#include "Engine/renderer/particles/spawner/ParticlesRotationSpeedSpawner.h"
#include "Engine/renderer/particles/spawner/ParticlesSizeSpawner.h"
#include "Engine/renderer/particles/spawner/position/ParticlesCircleSpawner.h"
#include "Engine/renderer/particles/spawner/position/ParticlesEdgesSpawner.h"
#include "Engine/renderer/particles/spawner/position/ParticlesPolygonSpawner.h"
#include "Engine/renderer/particles/spawner/velocity/ParticlesMultiDirectionSpawner.h"
#include "Engine/renderer/particles/spawner/velocity/ParticlesRadialVelocitySpawner.h"
#include "Engine/renderer/particles/spawner/velocity/ParticlesRightVelSpawner.h"
#include "Engine/renderer/particles/spawner/velocity/ParticlesVectorVelSpawner.h"
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

        if (ImGui::Button("Add Spawner")) {
            ImGui::OpenPopup("AddSpawnerPopup");
        }

        if (ImGui::BeginPopup("AddSpawnerPopup")) {
            AddSpawner<nsParticlesAngleSpawner>(s);
            AddSpawner<nsParticlesCircleSpawner>(s);
            AddSpawner<nsParticlesColorSpawner>(s);
            AddSpawner<nsParticlesEdgesSpawner>(s);
            AddSpawner<nsParticlesPolygonSpawner>(s);
            AddSpawner<nsParticlesLifeSpawner>(s);
            AddSpawner<nsParticlesMultiDirectionSpawner>(s);
            AddSpawner<nsParticlesRadialVelSpawner>(s);
            AddSpawner<nsParticlesRightVelSpawner>(s);
            AddSpawner<nsParticlesSizeSpawner>(s);
            AddSpawner<nsParticlesVectorVelSpawner>(s);
            AddSpawner<nsParticlesRotationSpeedSpawner>(s);
            ImGui::EndPopup();
        }

        int buttonId = 0;
        for (auto &child : s->list) {
            ImGui::SeparatorText(child->GetTitle());

            context->DrawProps(child);

            nsString buttonLabel;
            buttonLabel.Format("Remove Spawner##%d", buttonId++);
            if (ImGui::Button(buttonLabel)) {
                nsUndoService::Shared()->Push(new nsUndoVectorRemove(s->list, child));
                break;
            }
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
        if (dynamic_cast<nsParticlesPolygonSpawner*>(spawner)) {
            return false;
        }
        return dynamic_cast<nsParticlesEdgesSpawner*>(spawner);
    }
    void Draw(nsParticlesSpawner *spawner, nsPropsContext *context) override {
        const auto s = dynamic_cast<nsParticlesEdgesSpawner*>(spawner);
        if (ImGui::Button("Add Edge")) {
            const auto batch = new nsUndoBatch();
            batch->Add(new nsUndoArrayAdd(s->points, std::make_shared<nsPoint>(nsVec2())));
            batch->Add(new nsUndoArrayAdd(s->points, std::make_shared<nsPoint>(nsVec2())));
            nsUndoService::Shared()->Push(batch);
        }

        nsString title;

        const int numEdges = s->points.Size() / 2;
        for (int i = 0; i < numEdges; ++i) {
            auto &p1 = s->points[i * 2];
            auto &p2 = s->points[i * 2 + 1];

            ImGui::Separator();
            title.Format("Remove##point%d", i);
            if (ImGui::Button(title)) {
                const auto batch = new nsUndoBatch();
                batch->Add(new nsUndoArrayRemove(s->points, p1));
                batch->Add(new nsUndoArrayRemove(s->points, p2));
                nsUndoService::Shared()->Push(batch);
                break;
            } else {
                title.Format("Point 1##%d", i);
                nsVec2InputUndo<nsPoint>::DrawField( title, *p1.get());

                title.Format("Point 2##%d", i);
                nsVec2InputUndo<nsPoint>::DrawField( title, *p2.get());
            }
        }
    }
};

class nsPolygonSpawnerPropsView : public nsSpawnerPropsView {
public:
    bool IsSupported(nsParticlesSpawner *object) override {
        return dynamic_cast<nsParticlesPolygonSpawner*>(object);
    }

    void Draw(nsParticlesSpawner *object, nsPropsContext *context) override {
        const auto s = dynamic_cast<nsParticlesPolygonSpawner*>(object);

        if (ImGui::Button("Add Point")) {
            const auto batch = new nsUndoBatch();
            batch->Add(new nsUndoArrayAdd(s->points, std::make_shared<nsPoint>(nsVec2())));
            nsUndoService::Shared()->Push(batch);
        }

        nsString title;

        for (int i = 0; i < s->points.Size(); ++i) {
            auto &p = s->points[i];

            ImGui::Separator();
            title.Format("Remove##point%d", i);
            if (ImGui::Button(title)) {
                nsUndoService::Shared()->Push(new nsUndoArrayRemove(s->points, p));
                break;
            } else {
                title.Format("Point %d", i);
                nsVec2InputUndo<nsPoint>::DrawField( title, *p.get());
            }
        }
    }
};

class nsLifeSpawnerPropsView : public nsSpawnerPropsView {
    bool IsSupported(nsParticlesSpawner *spawner) override {
        return dynamic_cast<nsParticlesLifeSpawner*>(spawner);
    }
    void Draw(nsParticlesSpawner *spawner, nsPropsContext *context) override {
        const auto s = dynamic_cast<nsParticlesLifeSpawner*>(spawner);

        _minTime.Draw(s->minLifeTime);
        _maxTime.Draw(s->maxLifeTime);
    }

    nsFloatInputUndo<float> _minTime = "Min Time";
    nsFloatInputUndo<float> _maxTime = "Max Time";
};

class nsMultiDirSpawnerPropsView : public nsSpawnerPropsView {
public:
    bool IsSupported(nsParticlesSpawner *object) override {
        return dynamic_cast<nsParticlesMultiDirectionSpawner*>(object);
    }

    void Draw(nsParticlesSpawner *object, nsPropsContext *context) override {
        const auto s = dynamic_cast<nsParticlesMultiDirectionSpawner*>(object);
        if (ImGui::Button("Add Direction")) {
            nsUndoService::Shared()->Push(new nsUndoVectorAdd(s->directions, std::make_shared<nsParticlesMultiDirectionSpawner::Direction>()));
        }

        for (int i = 0; i < s->directions.size(); ++i) {
            auto &d = s->directions[i];
            nsString title;

            ImGui::Text("Direction %d", i);

            title.Format("Min Angle##%d", i);
            nsAngleInputUndo<float>::DrawField(title, d->minAngle);
            title.Format("Max Angle##%d", i);
            nsAngleInputUndo<float>::DrawField(title, d->maxAngle);
            title.Format("Min Speed##%d", i);
            nsFloatInputUndo<float>::DrawField(title, d->minSpeed);
            title.Format("Max Speed##%d", i);
            nsFloatInputUndo<float>::DrawField(title, d->maxSpeed);

            title.Format("Remove Direction##%d", i);
            if (ImGui::Button(title)) {
                nsUndoService::Shared()->Push(new nsUndoVectorRemove(s->directions, d));
                break;
            }
            ImGui::Separator();
        }

    }
};

class nsRadialVelSpawnerPropsView : public nsSpawnerPropsView {
    bool IsSupported(nsParticlesSpawner *object) override {
        return dynamic_cast<nsParticlesRadialVelSpawner*>(object);
    }

    void Draw(nsParticlesSpawner *object, nsPropsContext *context) override {
        const auto s = dynamic_cast<nsParticlesRadialVelSpawner*>(object);

        nsFloatInputUndo<float>::DrawField("Min Speed##radial vel", s->minSpeed);
        nsFloatInputUndo<float>::DrawField("Max Speed##radial vel", s->maxSpeed);
        nsBoolInputUndo<bool>::DrawField("Random Direction##radial vel", s->randomDirection);
    }
};

class nsRightVelSpawnerPropsView final : public nsSpawnerPropsView {
    bool IsSupported(nsParticlesSpawner *object) override {
        return dynamic_cast<nsParticlesRightVelSpawner*>(object);
    }

    void Draw(nsParticlesSpawner *object, nsPropsContext *context) override {
        const auto s = dynamic_cast<nsParticlesRightVelSpawner*>(object);

        nsFloatInputUndo<float>::DrawField("Min Speed##right vel", s->minSpeed);
        nsFloatInputUndo<float>::DrawField("Max Speed##right vel", s->maxSpeed);
        nsBoolInputUndo<bool>::DrawField("Random Direction##right vel", s->randomDirection);
    }
};

class nsSizeSpawnerPropsView final : public nsSpawnerPropsView {
    bool IsSupported(nsParticlesSpawner *object) override {
        return dynamic_cast<nsParticlesSizeSpawner*>(object);
    }

    void Draw(nsParticlesSpawner *object, nsPropsContext *context) override {
        const auto s = dynamic_cast<nsParticlesSizeSpawner*>(object);
        nsFloatInputUndo<float>::DrawField("Min Size##size_spawner", s->minSize);
        nsFloatInputUndo<float>::DrawField("Max Size##size_spawner", s->maxSize);
    }
};

class nsVectorVelSpawnerPropsView final : public nsSpawnerPropsView {
protected:
    bool IsSupported(nsParticlesSpawner *spawner) override {
        return dynamic_cast<nsParticlesVectorVelSpawner*>(spawner);
    }

    void Draw(nsParticlesSpawner *object, nsPropsContext *context) override {
        const auto s = dynamic_cast<nsParticlesVectorVelSpawner*>(object);

        nsFloatInputUndo<float>::DrawField("Min Speed##vector vel", s->minSpeed);
        nsFloatInputUndo<float>::DrawField("Max Speed##vector vel", s->maxSpeed);
        nsVec2InputUndo<nsVec2>::DrawField("Direction##vector vel", s->direction);
        nsBoolInputUndo<bool>::DrawField("Random Direction##vector vel", s->randomDirection);
    }
};

class nsRandomRotationSpawnerPropsView final : public nsSpawnerPropsView {
public:
    bool IsSupported(nsParticlesSpawner *object) override {
        return dynamic_cast<nsParticlesRotationSpeedSpawner*>(object);
    }

    void Draw(nsParticlesSpawner *object, nsPropsContext *context) override {
        const auto s = dynamic_cast<nsParticlesRotationSpeedSpawner*>(object);
        nsAngleInputUndo<float>::DrawField("Min Speed##rotation speed", s->minSpeed);
        nsAngleInputUndo<float>::DrawField("Max Speed##rotation speed", s->maxSpeed);
        nsBoolInputUndo<bool>::DrawField("Random Direction##rotation speed", s->randomDirection);
    }
};

nsSpawnerPropsRegistry::nsSpawnerPropsRegistry() {
    _views.emplace_back(new nsMultiSpawnerPropsView());
    _views.emplace_back(new nsAngleSpawnerPropsView());
    _views.emplace_back(new nsCircleSpawnerPropsView());
    _views.emplace_back(new nsColorSpawnerPropsView());
    _views.emplace_back(new nsEdgesSpawnerPropsView());
    _views.emplace_back(new nsPolygonSpawnerPropsView());
    _views.emplace_back(new nsLifeSpawnerPropsView());
    _views.emplace_back(new nsMultiDirSpawnerPropsView());
    _views.emplace_back(new nsRadialVelSpawnerPropsView());
    _views.emplace_back(new nsRightVelSpawnerPropsView());
    _views.emplace_back(new nsSizeSpawnerPropsView());
    _views.emplace_back(new nsVectorVelSpawnerPropsView());
    _views.emplace_back(new nsRandomRotationSpawnerPropsView());
}
