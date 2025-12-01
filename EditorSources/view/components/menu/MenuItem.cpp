//
// Created by Roman on 11/28/2025.
//

#include "MenuItem.h"

#include "MenuSeparator.h"
#include "imgui/imgui.h"
nsMenuItem::nsMenuItem(const char *label) : _label(label) {
}

nsMenuItem* nsMenuItem::Shortcut(const char *label, const ImGuiKeyChord hotkeys, const bool global) {
    _shortcut = label;
    _hotkeys = hotkeys;
    _global = global;
    return this;
}

nsMenuItem* nsMenuItem::AddItem(const char *label) {
    auto item = new nsMenuItem(label);
    _children.emplace_back(item);
    return item;
}

void nsMenuItem::AddSeparator() {
    _children.emplace_back(new nsMenuSeparator());
}

void nsMenuItem::Draw() {
    if (_children.empty()) {
        if (ImGui::MenuItem(_label.c_str(), _shortcut.empty() ? nullptr : _shortcut.c_str(), selected, enabled)) {
            if (_action) {
                _action();
            }
        }
    } else {
        if (ImGui::BeginMenu(_label.c_str(), enabled)) {
            for (const auto &child: _children) {
                child->Draw();
            }
            ImGui::EndMenu();
        }
    }
}

void nsMenuItem::Update() {
    nsBaseMenuItem::Update();
    if (!_children.empty()) {
        for (const auto &child: _children) {
            child->Update();
        }
    } else if (_hotkeys) {
        if (ImGui::Shortcut(_hotkeys, _global ? ImGuiInputFlags_RouteGlobal : 0)) {
            if (_action) {
                _action();
            }
        }
    }
}
