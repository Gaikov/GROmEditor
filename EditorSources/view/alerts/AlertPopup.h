// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file AlertPopup.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "view/BaseView.h"

class nsAlertPopup : public nsBaseView {
public:
    nsAlertPopup(const char *title, const char *message);

protected:
    std::string _title;
    std::string _message;
    std::string _id;

    bool _show = true;

    void Draw() override;
};

