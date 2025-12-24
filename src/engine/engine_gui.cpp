#include "engine_gui.h"
#include "engine.h"
#include "light.h"

#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void TestGUI::init(const Window &window, Engine &engineRef) {
    engine = &engineRef;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

    ImGui_ImplGlfw_InitForOpenGL(window.window, true);
    ImGui_ImplOpenGL3_Init();
}


void TestGUI::begin_frame() const {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Light Editor");

    ImGui::ColorEdit3("Ambient Background", glm::value_ptr(engine->background_rgb));
    for (size_t i = 0; i < engine->light_render_objects.size(); ++i) {
        ImGui::PushID(static_cast<int>(i));
        ImGui::Text("Light %d", static_cast<int>(i));
        ImGui::Checkbox("Visible", &engine->light_render_objects[i].visible);
        ImGui::SliderFloat3("Position", &engine->light_render_objects[i].light.position.x, -100.0f, 100.0f);
        ImGui::ColorEdit3("Color", &engine->light_render_objects[i].light.color.x);
        ImGui::SliderFloat("Intensity", &engine->light_render_objects[i].light.intensity, 0.0f, 100.0f);
        if (ImGui::Button("Remove Light")) {
            engine->light_render_objects.erase(engine->light_render_objects.begin() + i);
            ImGui::PopID();
            break;
        }
        ImGui::Separator();
        ImGui::PopID();
    }
    if (ImGui::Button("Add Light")) {
        engine->light_render_objects.emplace_back();
    }

    ImGui::End();

    ImGui::Begin("Render Settings");

    ImGui::Checkbox("Draw Wireframe", &engine->render_settings.wireframe);

    ImGui::End();

    engine->registry.draw_gui();
}

void TestGUI::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
