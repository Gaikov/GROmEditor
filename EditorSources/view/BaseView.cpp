//
// Created by Roman on 11/13/2025.
//

#include "BaseView.h"

#include "nsLib/locator/ServiceLocator.h"

nsBaseView::nsBaseView() {
    _model = Locate<nsSVModel>();
}
