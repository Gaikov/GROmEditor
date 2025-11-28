// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file MenuItem.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include <functional>
#include <memory>
#include <string>

#include "BaseMenuItem.h"

class nsMenuItem : public nsBaseMenuItem {
public:
    explicit nsMenuItem(const char *label);

    void Shortcut(const char *label) { _shortcut = label; }
    void SetCallback(const std::function<void()> &callback) { _callback = callback; }
    nsMenuItem* AddItem(const char *label);

    void Draw() override;

protected:
    std::string _label;
    std::string _shortcut;
    std::function<void()> _callback;

    std::vector<sp_t> _children;

};
