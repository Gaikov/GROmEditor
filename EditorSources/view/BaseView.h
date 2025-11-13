// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file BaseView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include <memory>

class nsBaseView {
public:
    typedef std::shared_ptr<nsBaseView> view_t;

    virtual ~nsBaseView() = default;

    bool visible = true;
    virtual void Draw() = 0;
};
