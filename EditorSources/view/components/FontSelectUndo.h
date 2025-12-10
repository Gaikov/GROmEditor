// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file FontSelectUndo.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "BaseAssetSelect.h"
#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVarChange.h"
#include "Engine/Font.h"
#include "Engine/renderer/font/FontsCache.h"

template<typename TFontVar>
class nsFontSelectUndo final : public nsBaseAssetSelect {
    static constexpr auto IMAGE_SIZE = 300;
public:
    nsFontSelectUndo() {
        _extensions = {"fnt"};
        _popupSize = ImVec2(IMAGE_SIZE * 1.5f, IMAGE_SIZE);
    };

    void Draw(TFontVar &var) {
        _current = var;

        nsString path;
        if (_current) {
            path = _current->GetPath();
        }

        DrawInputField("Font", path);

        if (DrawSelectionPopup(path)) {
            if (_current != _selected) {
                nsUndoService::Shared()->Push(new nsUndoVarChange(var, _selected));
            }
        }
    }
protected:
    const char * GetPopupId() override;
    void OnClickBrowse() override;
    void DrawSelectedInfo() override;
    void DrawSelectedPreview() override;
    void OnClickSelectPreview(const nsFilePath &path) override;
    ITexture* GetSelectedPage() const;

private:
    nsFont *_current = nullptr;
    nsFont *_selected = nullptr;
};

template<typename TFontVar>
const char * nsFontSelectUndo<TFontVar>::GetPopupId() {
    return "font_select_popup";
}

template<typename TFontVar>
void nsFontSelectUndo<TFontVar>::OnClickBrowse() {
    _selected = _current;
}

template<typename TFontVar>
void nsFontSelectUndo<TFontVar>::DrawSelectedInfo() {
    if (_selected) {
        if (const auto page = GetSelectedPage()) {
            int w, h;
            page->GetSize(w, h);
            ImGui::Text("%s: [%dx%d]", _selected->GetPath(), w, h);
        } else {
            ImGui::TextColored(ImVec4(1, 0, 0, 0), "Invalid font!");
        }
    }
}

template<typename TFontVar>
void nsFontSelectUndo<TFontVar>::DrawSelectedPreview() {
    auto page = GetSelectedPage();
    const auto id = page ? page->GetId() : 0;
    ImGui::Image(static_cast<intptr_t>(id), ImVec2(IMAGE_SIZE, IMAGE_SIZE));
}

template<typename TFontVar>
void nsFontSelectUndo<TFontVar>::OnClickSelectPreview(const nsFilePath &path) {
    _selected = nsFontsCache::Shared()->LoadFont(path);
}

template<typename TFontVar>
ITexture * nsFontSelectUndo<TFontVar>::GetSelectedPage() const {
    if (_selected && _selected->GetPageCount() > 0) {
        return _selected->GetPage(0);
    }
    return nullptr;
}
