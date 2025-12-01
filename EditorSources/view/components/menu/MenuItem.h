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
#include "imgui/imgui.h"

class nsMenuItem : public nsBaseMenuItem {
public:
    typedef std::function<void()> action_t;

    bool enabled = true;
    bool selected = false;

    explicit nsMenuItem(const char *label);

    nsMenuItem* Shortcut(const char *label, ImGuiKeyChord hotkeys, bool global = true);
    nsMenuItem* Action(const action_t &callback) { _action = callback; return this; }
    nsMenuItem* AddItem(const char *label);
    void AddSeparator();

    void Draw() override;
    void Update() override;

protected:
    std::string _label;
    std::string _shortcut;
    ImGuiKeyChord _hotkeys = 0;
    bool _global = false;

    action_t _action;

    std::vector<sp_t> _children;

};
