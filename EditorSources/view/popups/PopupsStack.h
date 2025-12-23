// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file PopupsStack.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "Popup.h"
#include "nsLib/SubSystem.h"
#include "view/BaseView.h"

class nsPopupsStack : public nsSubSystem<nsPopupsStack> {
public:
    template <typename TPopup, class... Args>
    TPopup* AddPopup(Args&&... args) {
        auto popup = std::make_shared<TPopup>(std::forward<Args>(args)...);
        _addedPopups.push_back(popup);
        return popup.get();
    }

    void RemovePop(nsPopup *popup);
    void Draw();

private:
    std::vector<nsPopup::sp_t> _addedPopups;
    std::vector<nsPopup*> _removedPopups;
    std::vector<nsPopup::sp_t> _popups;
    std::vector<nsPopup*> _activePopups;
};
