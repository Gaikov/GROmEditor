//
// Created by Roman on 11/13/2025.
//

#include "TestView.h"

#include "models/SVModel.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "nsLib/color.h"
#include "nsLib/locator/ServiceLocator.h"

class CascadingModals {
private:
    bool first_modal_open = false;
    bool second_modal_open = false;

public:
    void Render() {
        ImGui::Begin("Main Window");

        if (ImGui::Button("Open First Modal")) {
            first_modal_open = true;
            ImGui::OpenPopup("First Modal");
        }

        ImGui::End();

        if (first_modal_open) {
            RenderFirstModal();
        }

        if (second_modal_open) {
            RenderSecondModal();
        }
    }

private:
    void RenderFirstModal() {
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal("First Modal", &first_modal_open,
                                  ImGuiWindowFlags_AlwaysAutoResize)) {

            ImGui::Text("First Modal Window");
            ImGui::Separator();

            if (ImGui::Button("Open Second Modal")) {
                second_modal_open = true;
                ImGui::OpenPopup("Second Modal");
            }

            ImGui::SameLine();
            if (ImGui::Button("Close First")) {
                first_modal_open = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void RenderSecondModal() {
        if (first_modal_open) { // Второе модальное только если открыто первое
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(ImVec2(center.x + 50, center.y + 50),
                                   ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

            if (ImGui::BeginPopupModal("Second Modal", &second_modal_open,
                                      ImGuiWindowFlags_AlwaysAutoResize)) {

                ImGui::Text("Second Modal Window");
                ImGui::Text("Opened from first modal");
                ImGui::Separator();

                if (ImGui::Button("Close Second")) {
                    second_modal_open = false;
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }
        }
    }
};

CascadingModals models;

void BasicDragDrop() {
    ImGui::Begin("Basic Drag & Drop");

    // Источник для перетаскивания
    ImGui::Button("Drag me!");
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
        // Устанавливаем данные для перетаскивания
        const char* payload_data = "Dragged button data";
        ImGui::SetDragDropPayload("MY_DND_TYPE", payload_data, strlen(payload_data) + 1);

        // Предпросмотр при перетаскивании
        ImGui::Text("Dragging button...");
        ImGui::EndDragDropSource();
    }

    // Цель для бросания
    ImGui::Button("Drop here");
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MY_DND_TYPE")) {
            const char* received_data = (const char*)payload->Data;
            printf("Received: %s\n", received_data);
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::End();
}

void DraggableMenuItem() {
    ImGui::Begin("Draggable MenuItem Example");

    if (ImGui::BeginMenu("File")) {
        // Обычный MenuItem
        if (ImGui::MenuItem("New")) {
            // Обработка клика
        }

        // Draggable MenuItem
        if (ImGui::MenuItem("Open")) {
            // Обработка клика
        }

        // Добавляем перетаскивание для MenuItem "Open"
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
            const char* payload_data = "OPEN_OPERATION";
            ImGui::SetDragDropPayload("MENU_ITEM", payload_data, strlen(payload_data) + 1);
            ImGui::Text("Dragging: Open");
            ImGui::EndDragDropSource();
        }

        ImGui::EndMenu();
    }

    // Область для бросания
    ImGui::Button("Drop Menu Items Here", ImVec2(-1, 100));
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MENU_ITEM")) {
            const char* received = (const char*)payload->Data;
            printf("Dropped menu item: %s\n", received);
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::End();
}

nsTestView::nsTestView() {
    _model->user.testView.AddHandler(nsPropChangedName::CHANGED, [this](const nsBaseEvent*) {
        visible = _model->user.testView;
    });
    visible = _model->user.testView;
}

void nsTestView::Draw() {
    models.Render();
    // 2. Показываем простое окно
    static bool show_another_window = false;
    {
        static float f = 0.1f;
        static int counter = 0;
        static nsColor clear_color;

        ImGui::Begin("Hello, world!"); // Начинаем окно

        ImGui::Text("This is some useful text."); // Текст
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f); // Слайдер
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Выбор цвета

        if (ImGui::Button("Button")) // Кнопка
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    // 3. Показываем другое окно
    if (show_another_window) {
        ImGui::Begin("Another Window", &show_another_window, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

    ImGui::ShowDemoWindow();

    auto ctx = ImGui::GetCurrentContext();


    // Показываем отладочную информацию о попапах
    ImGui::Begin("Popup Debug");


    ImGui::Text("OpenPopupStack: %d", ctx->OpenPopupStack.Size);
    for (int i = 0; i < ctx->OpenPopupStack.Size; i++) {
        ImGui::Text("Popup %d: '%d'", i, ctx->OpenPopupStack[i].PopupId);
    }

    ImGui::Separator();
    ImGui::Text("Current popup: %s",
        ImGui::IsPopupOpen("", ImGuiPopupFlags_AnyPopupId) ? "Some popup open" : "No popup open");

    // Тестовые кнопки
    static bool test_open = false;
    if (ImGui::Button("Test OpenPopup")) {
        ImGui::OpenPopup("TestPopup");
        test_open = true;
    }

    ImGui::SameLine();
    ImGui::Text("TestPopup open: %s", ImGui::IsPopupOpen("TestPopup") ? "YES" : "NO");

    // Тестовый попап
    if (ImGui::BeginPopup("TestPopup")) {
        ImGui::Text("Test popup content");
        if (ImGui::Button("Close")) {
            ImGui::CloseCurrentPopup();
            test_open = false;
        }
        ImGui::EndPopup();
    }

    ImGui::End();

    BasicDragDrop();

    DraggableMenuItem();
}
