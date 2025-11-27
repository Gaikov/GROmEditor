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

void nsSceneUtils::DrawOrigin(nsVisualObject2d *obj) {
    if (!obj) return;

    auto &l = obj->origin;
    constexpr float size = 10.0f;

    const nsColor c2 = nsColor::magenta;

    const auto pos = l.ToGlobal(nsVec2(0, 0));

    const auto v1 = pos + nsVec2(-size, 0);
    const auto v2 = pos + nsVec2(size, 0);
    const auto v3 = pos + nsVec2(0, -size);
    const auto v4 = pos + nsVec2(0, size);

    RX_DrawLine(v1, v2, c2);
    RX_DrawLine(v3, v4, c2);
}
