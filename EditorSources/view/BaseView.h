// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file BaseView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include <memory>

class nsBaseView {
    friend class nsViewsRoot;

public:
    typedef std::shared_ptr<nsBaseView> view_t;

    virtual ~nsBaseView() = default;

    bool visible = true;

protected:
    virtual void Draw() = 0;
};
