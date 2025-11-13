//
// Created by Roman on 11/13/2025.
//

#include "TestView.h"

#include "SVModel.h"
#include "imgui/imgui.h"
#include "nsLib/color.h"
#include "nsLib/locator/ServiceLocator.h"

nsTestView::nsTestView() {
    const auto model = Locate<nsSVModel>();
    model->testView.AddHandler(nsPropChangedName::CHANGED, [this, model](const nsBaseEvent*) {
        visible = model->testView;
    });
    visible = model->testView;
}

void nsTestView::Draw() {
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
}
