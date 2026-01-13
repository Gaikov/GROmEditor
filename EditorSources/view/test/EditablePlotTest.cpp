//
// Created by Roman on 1/13/2026.
//

#include "EditablePlotTest.h"

#include "imgui/imgui.h"
#include "imgui/implot.h"
#include "nsLib/log.h"

EditablePlot::EditablePlot() {
    points.push_back(nsVec2(0, 0));
    points.push_back(nsVec2(0.5, 0.5));
}

void EditablePlot::Draw() {
    // Рисуем сетку и оси
    bool open = true;
    ImGui::Begin("Editable plot", &open, 0);

    if (ImPlot::BeginPlot("Editable line")) {
        ImPlot::SetupAxesLimits(0, 1, 0, 2);

        // Рисуем линию
        if (points.size() > 1) {
            xs.resize(points.size());
            ys.resize(points.size());
            for (int i = 0; i < points.size(); ++i) {
                xs[i] = points[i].x;
                ys[i] = points[i].y;
            }
            ImPlot::PlotLine("Line", xs.data(), ys.data(), (int)points.size());
        }

        // Делаем точки перетаскиваемыми
        for (size_t i = 0; i < points.size(); ++i) {
            double x = points[i].x, y = points[i].y;
            if (ImPlot::DragPoint(i, &x, &y,
                             ImVec4(1, 0.5f, 0, 1), 6.0f)) {
                points[i] = nsVec2(x, y);
            }

        }

        ImPlot::EndPlot();
    }

    ImGui::End();
}
