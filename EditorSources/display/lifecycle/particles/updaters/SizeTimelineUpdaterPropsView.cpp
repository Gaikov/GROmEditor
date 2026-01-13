//
// Created by Roman on 1/13/2026.
//

#include "SizeTimelineUpdaterPropsView.h"

#include "Engine/renderer/particles/updater/ParticlesSizeUpdater.h"
#include "Engine/renderer/particles/updater/size/ParticlesSizeTimelineUpdater.h"
#include "imgui/implot.h"

bool nsSizeTimelineUpdaterPropsView::IsSupported(nsParticlesUpdater *object) {
    return dynamic_cast<nsParticlesSizeTimelineUpdater*>(object);
}

void nsSizeTimelineUpdaterPropsView::Draw(nsParticlesUpdater *object, nsPropsContext *context) {
    const auto u = dynamic_cast<nsParticlesSizeTimelineUpdater*>(object);

    if (ImPlot::BeginPlot("Size Curve")) {
        ImPlot::SetupAxesLimits(0, 1, 0, 2);

        const auto size = u->timeline.size();
        const auto &points = u->timeline;
        if (size > 1) {
            xs.resize(size);
            ys.resize(size);
            float x = 0;
            for (int i = 0; i < size; ++i) {
                x += points[i]->time;
                xs[i] = x;
                ys[i] = points[i]->data;
            }
            ImPlot::PlotLine("Line", xs.data(), ys.data(), size);
        }

        /*for (size_t i = 0; i < size; ++i) {
            double x = points[i].time, y = points[i].data;
            if (ImPlot::DragPoint(i, &x, &y,
                             ImVec4(1, 0.5f, 0, 1), 6.0f)) {
                points[i] = nsVec2(x, y);
                             }

        }    */

        ImPlot::EndPlot();
    }
}
