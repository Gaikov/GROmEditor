// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file BaseMenuItem.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include <memory>

class nsBaseMenuItem {
public:
    typedef std::shared_ptr<nsBaseMenuItem> sp_t;

    virtual ~nsBaseMenuItem() = default;
    virtual void Draw() = 0;
    virtual void Update() {};
};
