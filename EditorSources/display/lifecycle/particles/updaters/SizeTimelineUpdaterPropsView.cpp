//
// Created by Roman on 1/13/2026.
//

#include "SizeTimelineUpdaterPropsView.h"

#include "Engine/renderer/particles/updater/ParticlesSizeUpdater.h"
#include "Engine/renderer/particles/updater/size/ParticlesSizeTimelineUpdater.h"
#include "imgui/implot.h"

static constexpr auto DEFAULT_POINT_COLOR = ImVec4(1, 0.5f, 0, 1);
static constexpr auto SELECTED_POINT_COLOR = ImVec4(0, 1, 0, 1);
static constexpr auto POINT_SIZE = 6.0f;

bool nsSizeTimelineUpdaterPropsView::IsSupported(nsParticlesUpdater *object) {
    return dynamic_cast<nsParticlesSizeTimelineUpdater *>(object);
}

void nsSizeTimelineUpdaterPropsView::Draw(nsParticlesUpdater *object, nsPropsContext *context) {
    const auto u = dynamic_cast<nsParticlesSizeTimelineUpdater *>(object);

    if (ImPlot::BeginPlot("Scale Curve")) {
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
            ImPlot::PlotLine("Scale", xs.data(), ys.data(), static_cast<int>(size));
        }

        float prev, next;
        for (int i = 0; i < size; ++i) {
            if (i == 0) {
                prev = 0;
                next = xs[1];
            } else if (i == size - 1) {
                prev = xs[i - 1];
                next = 1;
            } else {
                prev = xs[i - 1];
                next = xs[i + 1];
            }

            double x = xs[i];
            double y = ys[i];

            const auto color = _selectedPoint == i ? SELECTED_POINT_COLOR : DEFAULT_POINT_COLOR;

            if (ImPlot::DragPoint(i, &x, &y, color, POINT_SIZE)) {
                x = nsMath::Clamp(static_cast<float>(x), prev, next);
                const auto time = static_cast<float>(x - prev);
                const auto timeNext = static_cast<float>(next - x);
                if (!i) {
                    points[i]->time = 0;
                } else if (i == size - 1) {
                    points[i]->time = static_cast<float>(1.0 - prev);
                } else {
                    points[i]->time = time;
                }
                points[i]->data = static_cast<float>(std::max(y, 0.0));
                if (i && i + 1 < size) {
                    points[i + 1]->time = timeNext;
                }

                _selectedPoint = i;
            }

            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                const ImVec2 screen_pos = ImPlot::PlotToPixels(ImPlotPoint(x, y));
                const ImVec2 current_mouse = ImGui::GetMousePos();

                const float dx = nsMath::Absf(screen_pos.x - current_mouse.x);
                const float dy = nsMath::Absf(screen_pos.y - current_mouse.y);
                if (dx <= POINT_SIZE && dy <= POINT_SIZE) {
                    _selectedPoint = i;
                }
            }
        }

        ImPlot::EndPlot();
    }
}
