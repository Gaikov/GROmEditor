//
// Created by Roman on 1/15/2026.
//

#include "ColorTimelineUpdaterPropsView.h"

#include "Core/undo/UndoBatch.h"
#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVarPrevChange.h"
#include "Core/undo/UndoVectorInsert.h"
#include "Core/undo/UndoVectorRemove.h"
#include "imgui/imgui.h"
#include "imgui/implot.h"
#include "utils/GuiUtils.h"
#include "view/components/ColorInputUndo.h"

static constexpr auto DEFAULT_LINE_COLOR = ImVec4(1, 0.5f, 0, 1);
static constexpr auto SELECTED_LINE_COLOR = ImVec4(0, 1, 0, 1);
static constexpr auto OUTLINE_COLOR = ImVec4(0, 0, 0, 1);
static constexpr auto LINE_SIZE = 2.0f;

bool nsColorTimelineUpdaterPropsView::IsSupported(nsParticlesUpdater *object) {
    return dynamic_cast<nsParticlesColorTimelineUpdater *>(object);
}

void nsColorTimelineUpdaterPropsView::Draw(nsParticlesUpdater *object, nsPropsContext *context) {
    const auto u = dynamic_cast<nsParticlesColorTimelineUpdater *>(object);

    if (ImPlot::BeginPlot("Color Timeline", ImVec2(-1, 200), ImPlotFlags_NoMenus)) {
        bool clicked = false;
        bool hovered = false;
        int draggedPoint = -1;
        ImPlot::SetupAxesLimits(-0.1, 1.1, 0, 1, ImGuiCond_Always);

        const auto size = u->timeline.size();
        const auto &points = u->timeline;
        if (size > 1) {
            const auto drawList = ImPlot::GetPlotDrawList();

            for (int i = 0; i < size - 1; ++i) {
                auto &p1 = points[i];
                auto &p2 = points[i + 1];

                const auto c1 = nsGuiUtils::ColorU32(p1->data);
                const auto c2 = nsGuiUtils::ColorU32(p2->data);

                auto v1 = ImPlot::PlotToPixels(ImPlotPoint(p1->time, 0));
                auto v2 = ImPlot::PlotToPixels(ImPlotPoint(p2->time, 1));

                drawList->AddRectFilledMultiColor(v1, v2, c1, c2, c2, c1);
            }
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
                min = points[i - 1]->time;
                max = points[i + 1]->time;
            }

            double x = points[i]->time;

            const auto color = _selectedPoint == i ? SELECTED_LINE_COLOR : DEFAULT_LINE_COLOR;

            ImPlot::DragLineX(i, &x, OUTLINE_COLOR, LINE_SIZE + 2, ImPlotDragToolFlags_NoInputs);


            if (ImPlot::DragLineX(i, &x, color, LINE_SIZE, 0, &clicked, &hovered)) {
                draggedPoint = i;
                _selectedPoint = i;
                if (_draggingPoint != i) {
                    _draggingPoint = i;
                    OnDragStarted(u, i);
                }

                x = nsMath::Clamp(static_cast<float>(x), min, max);
                points[i]->time = static_cast<float>(x);
            }

            if (clicked) {
                _selectedPoint = i;
            }
        }

        if (_draggingPoint != -1 && _draggingPoint != draggedPoint) {
            OnDragStopped(u, _draggingPoint);
            _draggingPoint = -1;
        }

        if (!hovered && !clicked && draggedPoint < 0 && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            const auto pos = ImPlot::GetPlotMousePos();
            if (nsMath::InRange(pos.x, 0, 1) && nsMath::InRange(pos.y, 0, 1)) {
                Log::Info("Add point: %f %f", pos.x, pos.y);
                for (auto i = 0; i < size; ++i) {
                    if (points[i]->time > pos.x) {
                        const auto frame = std::make_shared<nsParticlesColorTimelineUpdater::Frame>();
                        frame->time = pos.x;
                        nsUndoService::Shared()->Push(new nsUndoVectorInsert(u->timeline, i, frame));
                        break;
                    }
                }
            }
        }

        ImPlot::EndPlot();

        if (_selectedPoint >= 0 && _selectedPoint < u->timeline.size()) {
            _colorInput.Draw(u->timeline[_selectedPoint]->data);
            if (_selectedPoint > 0 && _selectedPoint < u->timeline.size() - 1) {
                ImGui::SameLine();
                if (ImGui::Button("Remove##Color")) {
                    nsUndoService::Shared()->Push(new nsUndoVectorRemove(u->timeline, u->timeline[_selectedPoint]));
                }
            }
        }
    }
}

void nsColorTimelineUpdaterPropsView::OnDragStarted(const nsParticlesColorTimelineUpdater *object, const int pointIndex) {
    Log::Info("Drag started");
    _prevValue = *object->timeline[pointIndex];
}

void nsColorTimelineUpdaterPropsView::OnDragStopped(const nsParticlesColorTimelineUpdater *object, const int pointIndex) const {
    Log::Info("Drag stopped");

    const auto batch = new nsUndoBatch();
    const auto &frame = object->timeline[pointIndex];
    batch->Add(new nsUndoVarPrevChange(frame->time, _prevValue.time));
    batch->Add(new nsUndoVarPrevChange(frame->data, _prevValue.data));

    nsUndoService::Shared()->Push(batch);
}