//
// Created by Roman on 1/13/2026.
//

#include "SizeTimelineUpdaterPropsView.h"

#include "Core/undo/UndoBatch.h"
#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVarPrevChange.h"
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
        int draggedPoint = -1;
        ImPlot::SetupAxesLimits(0, 1, 0, 2);

        const auto size = u->timeline.size();
        const auto &points = u->timeline;
        if (size > 1) {
            _xs.resize(size);
            _ys.resize(size);

            for (int i = 0; i < size; ++i) {
                _xs[i] = points[i]->time;
                _ys[i] = points[i]->data;
            }
            ImPlot::PlotLine("Scale", _xs.data(), _ys.data(), static_cast<int>(size));
        }

        float min, max;
        for (int i = 0; i < size; ++i) {
            if (i == 0) {
                min = 0;
                max = 0;
            } else if (i == size - 1) {
                min = 1;
                max = 1;
            } else {
                min = _xs[i - 1];
                max = _xs[i + 1];
            }

            double x = _xs[i];
            double y = _ys[i];

            const auto color = _selectedPoint == i ? SELECTED_POINT_COLOR : DEFAULT_POINT_COLOR;

            if (ImPlot::DragPoint(i, &x, &y, color, POINT_SIZE)) {
                draggedPoint = i;
                _selectedPoint = i;
                if (_draggingPoint != i) {
                    _draggingPoint = i;
                    OnDragStarted(u, i);
                }

                x = nsMath::Clamp(static_cast<float>(x), min, max);
                points[i]->time = static_cast<float>(x);
                points[i]->data = static_cast<float>(std::max(y, 0.0));
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

        if (_draggingPoint != -1 && _draggingPoint != draggedPoint) {
            OnDragStopped(u, _draggingPoint);
            _draggingPoint = -1;
        }

        ImPlot::EndPlot();
    }
}

void nsSizeTimelineUpdaterPropsView::OnDragStarted(const nsParticlesSizeTimelineUpdater *object, int pointIndex) {
    Log::Info("Drag started");
    _prevValue = *object->timeline[pointIndex];
}

void nsSizeTimelineUpdaterPropsView::OnDragStopped(const nsParticlesSizeTimelineUpdater *object, int pointIndex) const {
    Log::Info("Drag stopped");

    const auto batch = new nsUndoBatch();
        const auto &frame = object->timeline[pointIndex];
        batch->Add(new nsUndoVarPrevChange(frame->time, _prevValue.time));
        batch->Add(new nsUndoVarPrevChange(frame->data, _prevValue.data));

    nsUndoService::Shared()->Push(batch);
}
