// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file BaseView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include <memory>

#include "models/AppModel.h"

class nsBaseView {
    friend class nsViewsRoot;

public:
    typedef std::shared_ptr<nsBaseView> view_t;

    nsBaseView();
    virtual ~nsBaseView() = default;

    bool visible = true;

protected:
    nsAppModel* const _model;

    virtual void Draw() = 0;
    virtual void PostDraw() {}
};
