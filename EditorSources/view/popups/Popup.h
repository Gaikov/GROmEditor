// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file Popup.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include <memory>
#include <string>

#include "imgui/imgui.h"

class nsPopup {
    friend class nsPopupsStack;
public:
    typedef std::shared_ptr<nsPopup> sp_t;

    nsPopup();
    virtual ~nsPopup() = default;
    void SetTitle(const char *title);

protected:
    ImGuiWindowFlags _popupFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize;

    virtual bool BeginPopup();
    virtual void DrawContent() = 0;
    virtual void EndPopup();

private:
    int _id = 0;
    std::string _title;

    bool _shouldOpen = true;
    bool _isOpen = true;
};


