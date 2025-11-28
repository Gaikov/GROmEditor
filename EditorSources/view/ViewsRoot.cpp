//
// Created by Roman on 11/13/2025.
//

#include "ViewsRoot.h"

#include "MainMenuBar.h"
#include "TestView.h"
#include "TopBarView.h"
#include "library/LibraryView.h"
#include "library/ScenePropsView.h"

bool nsViewsRoot::OnInit() {
    nsSubSystem::OnInit();

    AddView<nsMainMenuBar>();
    AddView<nsTestView>();
    AddView<nsLibraryView>();
    AddView<nsScenePropsView>();
    AddView<nsTopBarView>();

    return true;
}

void nsViewsRoot::RemoveView(const nsBaseView *view) {
    _removedViews.push_back(view);
}

void nsViewsRoot::Draw() {
    for (const auto& view : _views) {
        if (view->visible) {
            view->Draw();
        }
    }
    for (const auto& view : _views) {
        if (view->visible) {
            view->PostDraw();
        }
    }

    for (auto added : _addedViews) {
        _views.push_back(added);
    }
    _addedViews.clear();

    for (auto removed : _removedViews) {
        auto it = std::find_if(_views.begin(), _views.end(), [removed](const auto& v) { return v.get() == removed; });
        if (it != _views.end()) {
            _views.erase(it);
        }
    }
    _removedViews.clear();
}
