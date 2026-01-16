//
// Created by Roman on 1/13/2026.
//

#include "SizeTimelineUpdaterPropsView.h"

#include "Core/undo/UndoBatch.h"
#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVarPrevChange.h"
#include "Core/undo/UndoVectorInsert.h"
#include "Core/undo/UndoVectorRemove.h"
#include "Engine/renderer/particles/updater/ParticlesSizeUpdater.h"
#include "Engine/renderer/particles/updater/size/ParticlesSizeTimelineUpdater.h"
#include "imgui/implot.h"
#include "view/components/FloatInputUndo.h"

static constexpr auto DEFAULT_LINE_COLOR = ImVec4(1, 0.5f, 0, 1);
static constexpr auto SELECTED_LINE_COLOR = ImVec4(0, 1, 0, 1);
static constexpr auto LINE_SIZE = 6.0f;

bool nsSizeTimelineUpdaterPropsView::IsSupported(nsParticlesUpdater *object) {
    return dynamic_cast<nsParticlesSizeTimelineUpdater *>(object);
}

void nsSizeTimelineUpdaterPropsView::Draw(nsParticlesUpdater *object, nsPropsContext *context) {
    const auto u = dynamic_cast<nsParticlesSizeTimelineUpdater *>(object);

    if (ImPlot::BeginPlot("Scale Curve", ImVec2(-1, 0), ImPlotFlags_NoMenus)) {
        int draggedPoint = -1;
        bool pointClicked = false;
        ImPlot::SetupAxesLimits(0, 1, 0, 2);
        ImPlot::SetupAxes("Time", "Scale", ImPlotAxisFlags_LockMin | ImPlotAxisFlags_LockMax, ImPlotAxisFlags_LockMin);

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

            const auto color = _selectedPoint == i ? SELECTED_LINE_COLOR : DEFAULT_LINE_COLOR;

            if (ImPlot::DragPoint(i, &x, &y, color, LINE_SIZE, 0, &pointClicked)) {
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

            if (pointClicked) {
                _selectedPoint = i;
            }
        }

        if (_draggingPoint != -1 && _draggingPoint != draggedPoint) {
            OnDragStopped(u, _draggingPoint);
            _draggingPoint = -1;
        }

        if (!pointClicked) {
            if (ImGui::BeginPopupContextWindow("Size Scale Context")) {
                if (ImGui::IsWindowAppearing()) {
                    _addPos = ImPlot::GetPlotMousePos();
                }
                if (ImGui::MenuItem("Add Point##Scale")) {

                    for (auto i = 0; i < size; ++i) {
                        if (points[i]->time > _addPos.x) {
                            const auto frame = std::make_shared<nsParticlesSizeTimelineUpdater::Frame>();
                            frame->time = _addPos.x;
                            frame->data = _addPos.y;
                            nsUndoService::Shared()->Push(new nsUndoVectorInsert(u->timeline, i, frame));
                            break;
                        }
                    }
                }
                ImGui::EndPopup();
            }
        }

        ImPlot::EndPlot();

        if (_selectedPoint >= 0 && _selectedPoint < u->timeline.size()) {
            nsFloatInputUndo<float>::DrawField("Scale##Scale", u->timeline[_selectedPoint]->data);
            if (_selectedPoint > 0 && _selectedPoint < u->timeline.size() - 1) {
                ImGui::SameLine();
                if (ImGui::Button("Remove##Scale")) {
                    nsUndoService::Shared()->Push(new nsUndoVectorRemove(u->timeline, u->timeline[_selectedPoint]));
                }
            }
        }
    }
}

void nsSizeTimelineUpdaterPropsView::OnDragStarted(const nsParticlesSizeTimelineUpdater *object, const int pointIndex) {
    Log::Info("Drag started");
    _prevValue = *object->timeline[pointIndex];
}

void nsSizeTimelineUpdaterPropsView::OnDragStopped(const nsParticlesSizeTimelineUpdater *object, const int pointIndex) const {
    Log::Info("Drag stopped");

    const auto batch = new nsUndoBatch();
    const auto &frame = object->timeline[pointIndex];
    batch->Add(new nsUndoVarPrevChange(frame->time, _prevValue.time));
    batch->Add(new nsUndoVarPrevChange(frame->data, _prevValue.data));

    nsUndoService::Shared()->Push(batch);
}
