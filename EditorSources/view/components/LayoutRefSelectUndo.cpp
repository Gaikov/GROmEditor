//
// Created by Roman on 12/8/2025.
//

#include "LayoutRefSelectUndo.h"

#include "Core/undo/UndoPropertyChange.h"
#include "Core/undo/UndoService.h"

nsLayoutRefSelectUndo::nsLayoutRefSelectUndo() {
    _extensions = {"layout"};
    _popupSize = ImVec2(400, 300);
}

void nsLayoutRefSelectUndo::Draw(nsProperty<std::string> &source) {
    _passed = source;
    DrawInputField("Layout", _passed);
    if (DrawSelectionPopup(_passed)) {
        nsUndoService::Shared()->Push(new nsUndoPropertyChange<std::string>(source, _selected.AsChar()));
    }
}

const char * nsLayoutRefSelectUndo::GetPopupId() {
    return "LayoutRefSelectPopup";
}

void nsLayoutRefSelectUndo::OnClickBrowse() {
    _selected = _passed;
}

void nsLayoutRefSelectUndo::DrawSelectedInfo() {
    ImGui::Text(_selected);
}

void nsLayoutRefSelectUndo::DrawSelectedPreview() {
}

void nsLayoutRefSelectUndo::OnClickSelectPreview(const nsFilePath &path) {
    _selected = path;
}
