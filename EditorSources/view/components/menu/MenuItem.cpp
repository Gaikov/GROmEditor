//
// Created by Roman on 11/28/2025.
//

#include "MenuItem.h"

#include "imgui/imgui.h"
nsMenuItem::nsMenuItem(const char *label) : _label(label) {
}

nsMenuItem* nsMenuItem::AddItem(const char *label) {
    auto item = new nsMenuItem(label);
    _children.emplace_back(item);
    return item;
}

void nsMenuItem::Draw() {
    if (_children.empty()) {
        if (ImGui::MenuItem(_label.c_str())) {
            if (_callback) {
                _callback();
            }
        }
    } else {
        if (ImGui::BeginMenu(_label.c_str())) {
            for (const auto &child: _children) {
                child->Draw();
            }
            ImGui::EndMenu();
        }
    }
}
