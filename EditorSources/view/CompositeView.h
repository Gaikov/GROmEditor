// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file CompositeView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "BaseView.h"

class nsCompositeView : public nsBaseView {
protected:
    std::vector<sp_t> _views;

    template <typename TView, class... Args>
    sp_t AddView(Args&&... args) {
        auto view = std::make_shared<TView>(std::forward<Args>(args)...);
        _views.push_back(view);
        return view;
    }

    void Draw() override;
    void PostDraw() override;
};
