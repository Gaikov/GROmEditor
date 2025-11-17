//
// Created by Roman on 11/17/2025.
//

#include "Popup.h"

#include "PopupsStack.h"
#include "imgui/imgui.h"
#include "nsLib/log.h"

static int idCounter = 0;

nsPopup::nsPopup() {
    _id = idCounter++;
    SetTitle("Popup");
}

void nsPopup::SetTitle(const char *title) {
    _title = title;
    _title += "##Popup_" + std::to_string(_id);
}

bool nsPopup::BeginPopup() {
    if (_shouldOpen) {
        _shouldOpen = false;
        ImGui::OpenPopup(_title.c_str(), ImGuiPopupFlags_NoOpenOverExistingPopup);
        Log::Debug("Popup opened: %s", _title.c_str());
    }

    const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal(_title.c_str(), &_isOpen,
                                  ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)) {
        DrawContent();
        return true;
    }

    if (!_isOpen) {
        nsPopupsStack::Shared()->RemovePop(this);
        Log::Debug("Popup closed: %s", _title.c_str());
    }
    return false;
}

void nsPopup::EndPopup() {
    ImGui::EndPopup();

}
