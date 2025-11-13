// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ViewsRoot.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "BaseView.h"
#include "nsLib/SubSystem.h"

class nsViewsRoot : public nsSubSystem<nsViewsRoot> {
public:
    template <typename TView>
    nsBaseView::view_t AddView() {
        auto view = std::make_shared<TView>();
        _views.push_back(view);
        return view;
    }

    void RemoveView(const nsBaseView *view);
    void Draw() const;

private:
    std::vector<nsBaseView::view_t> _views;
};

