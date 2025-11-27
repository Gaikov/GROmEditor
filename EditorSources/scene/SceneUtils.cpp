//
// Created by Roman on 11/27/2025.
//

#include "SceneUtils.h"

#include "Engine/RenAux.h"

void nsSceneUtils::DrawBounds(nsVisualObject2d *obj) {
    if (!obj) return;

    nsRect rect;
    obj->GetLocalBounds(rect);

    auto &l = obj->origin;

    const auto v1 = l.ToGlobal(nsVec2(rect.minX(), rect.minY()));
    const auto v2 = l.ToGlobal(nsVec2(rect.minX(), rect.maxY()));
    const auto v3 = l.ToGlobal(nsVec2(rect.maxX(), rect.maxY()));
    const auto v4 = l.ToGlobal(nsVec2(rect.maxX(), rect.minY()));

    const nsColor c = nsColor::green;

    RX_DrawLine(v1, v2, c);
    RX_DrawLine(v3, v2, c);
    RX_DrawLine(v3, v4, c);
    RX_DrawLine(v1, v4, c);
}
