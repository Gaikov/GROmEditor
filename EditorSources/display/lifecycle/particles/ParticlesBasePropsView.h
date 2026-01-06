// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ParticlesBasePropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include <memory>
#include <vector>

template <class TTarget>
class nsBasePropsView {
    friend class nsSpawnerPropsRegistry;

public:
    class nsPropsContext {
    public:
        virtual ~nsPropsContext() = default;
        virtual void DrawProps(TTarget *object) = 0;
    };

    virtual ~nsBasePropsView() = default;

    virtual bool IsSupported(TTarget *object) = 0;
    virtual void Draw(TTarget *object, nsPropsContext *context) = 0;
};

template <class TTarget>
class nsBasePropsRegistry : public nsBasePropsView<TTarget>::nsPropsContext {
public:
    void DrawProps(TTarget *object) override {
        for (auto &v : _views) {
            if (v->IsSupported(object)) {
                v->Draw(object, this);
            }
        }
    }

protected:
    std::vector<std::unique_ptr<nsBasePropsView<TTarget>>> _views;
};
