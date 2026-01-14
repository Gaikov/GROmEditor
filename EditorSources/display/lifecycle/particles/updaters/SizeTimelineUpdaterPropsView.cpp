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
            float x = 0;
            for (int i = 0; i < size; ++i) {
                x += points[i]->time;
                _xs[i] = x;
                _ys[i] = points[i]->data;
            }
            ImPlot::PlotLine("Scale", _xs.data(), _ys.data(), static_cast<int>(size));
        }

        float prev, next;
        for (int i = 0; i < size; ++i) {
            if (i == 0) {
                prev = 0;
                next = _xs[1];
            } else if (i == size - 1) {
                prev = _xs[i - 1];
                next = 1;
            } else {
                prev = _xs[i - 1];
                next = _xs[i + 1];
            }

            double x = _xs[i];
            double y = _ys[i];

            const auto color = _selectedPoint == i ? SELECTED_POINT_COLOR : DEFAULT_POINT_COLOR;

            if (ImPlot::DragPoint(i, &x, &y, color, POINT_SIZE)) {
                draggedPoint = i;
                _selectedPoint = i;
                if (_draggingPoint != i) {
                    _draggingPoint = i;
                    OnDragStarted(u);
                }

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
            _draggingPoint = -1;
            OnDragStopped(u);
        }

        ImPlot::EndPlot();
    }
}

void nsSizeTimelineUpdaterPropsView::OnDragStarted(const nsParticlesSizeTimelineUpdater *object) {
    Log::Info("Drag started");

    _prevFrames.clear();
    for (auto &f : object->timeline) {
        _prevFrames.push_back(*f);
    }
}

void nsSizeTimelineUpdaterPropsView::OnDragStopped(const nsParticlesSizeTimelineUpdater *object) const {
    Log::Info("Drag stopped");

    const auto batch = new nsUndoBatch();
    for (int i = 0; i < _prevFrames.size(); ++i) {
        const auto &frame = object->timeline[i];
        const auto &prevValue = _prevFrames[i];

        batch->Add(new nsUndoVarPrevChange(frame->time, prevValue.time));
        batch->Add(new nsUndoVarPrevChange(frame->data, prevValue.data));
    }
    nsUndoService::Shared()->Push(batch);
}
