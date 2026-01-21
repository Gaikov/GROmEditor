//
// Created by Roman on 11/13/2025.
//

#include "ViewsRoot.h"

#include "MainMenuBar.h"
#include "TestView.h"
#include "TopBarView.h"
#include "library/LibraryView.h"
#include "library/ScenePropsView.h"

void nsViewsRoot::Draw() {
    nsCompositeView::Draw();
    PostDraw();
}

bool nsViewsRoot::OnInit() {
    nsSubSystem::OnInit();

    AddView<nsMainMenuBar>();
    AddView<nsTestView>();
    AddView<nsLibraryView>();
    AddView<nsScenePropsView>();
    AddView<nsTopBarView>();

    return true;
}
