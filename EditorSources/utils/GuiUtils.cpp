//
// Created by Roman on 1/16/2026.
//

#include "GuiUtils.h"

ImU32 nsGuiUtils::ColorU32(const nsColor &color) {
    return ImGui::ColorConvertFloat4ToU32(ImVec4(color.r, color.g, color.b, color.a));
}
