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
    template <typename TView, class... Args>
    nsBaseView::view_t AddView(Args&&... args) {
        auto view = std::make_shared<TView>(std::forward<Args>(args)...);
        _addedViews.push_back(view);
        return view;
    }

    void RemoveView(const nsBaseView *view);
    void Draw();

protected:
    bool OnInit() override;

private:
    std::vector<nsBaseView::view_t> _views;
    std::vector<const nsBaseView*> _removedViews;
    std::vector<nsBaseView::view_t> _addedViews;
};

