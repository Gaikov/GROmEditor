#include "AlertPopup.h"
#include "imgui/imgui.h"
#include "view/popups/PopupsStack.h"

nsAlertPopup::nsAlertPopup(const char *title, const char *message) {
    _message = message;
    SetTitle(title);
}

void nsAlertPopup::Info(const char *message) {
    nsPopupsStack::Shared()->AddPopup<nsAlertPopup>("Info", message);
}

void nsAlertPopup::Error(const char *message) {
    nsPopupsStack::Shared()->AddPopup<nsAlertPopup>("Error", message);
}

void nsAlertPopup::Warning(const char *message) {
    nsPopupsStack::Shared()->AddPopup<nsAlertPopup>("Warning", message);
}

void nsAlertPopup::DrawContent() {
    ImGui::Text("%s", _message.c_str());

    ImGui::SetItemDefaultFocus();
    if (ImGui::Button("Close")) {
        ImGui::CloseCurrentPopup();
    }
}
