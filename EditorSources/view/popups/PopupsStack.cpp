//
// Created by Roman on 11/17/2025.
//

#include "PopupsStack.h"

void nsPopupsStack::RemovePop(nsPopup *popup) {
    _removedPopups.push_back(popup);
}

void nsPopupsStack::Draw() {
    for (auto p: _addedPopups) {
        _popups.push_back(p);
    }
    _addedPopups.clear();

    for (auto removed: _removedPopups) {
        auto it = std::find_if(_popups.begin(), _popups.end(), [removed](const auto &v) { return v.get() == removed; });
        if (it != _popups.end()) {
            _popups.erase(it);
        }
    }
    _removedPopups.clear();

    _activePopups.clear();
    for (auto &p: _popups) {
        if (p->BeginPopup()) {
            _activePopups.push_back(p.get());
        } else {
            break;
        }
    }

    for (auto it = _activePopups.rbegin(); it != _activePopups.rend(); ++it) {
        (*it)->EndPopup();
    }
}
