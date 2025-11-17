// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file AlertPopup.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "view/popups/Popup.h"

class nsAlertPopup : public nsPopup {
public:
    nsAlertPopup(const char *title, const char *message);

    static void Info(const char *message);
    static void Error(const char *message);
    static void Warning(const char *message);

protected:
    std::string _message;

    void DrawContent() override;
};

