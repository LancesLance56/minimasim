#include "engine_gui.h"
#include "engine.h"
#include "lightpos.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void TestGUI::init(const Window& window, Engine& gameRef) {
    game = &gameRef;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

    ImGui_ImplGlfw_InitForOpenGL(window.window, true);
    ImGui_ImplOpenGL3_Init();

    // Set default curve
    auto& curve = game->get_curve();
    curve = {
        {0.0f, 0.0f, ImVec2(30, -0)},
        {0.25f, 0.1f, ImVec2(40, -10)},
        {0.5f, 0.2f, ImVec2(30, -20)},
        {0.75f, 0.4f, ImVec2(20, -20)},
        {1.0f, 1.0f, ImVec2(30, -30)}
    };

    editor = BezierEditor(window.window);
}


void TestGUI::begin_frame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Light Editor");

    for (size_t i = 0; i < global_lights.size(); ++i) {
        ImGui::PushID(static_cast<int>(i));
        ImGui::Text("Light %d", static_cast<int>(i));
        ImGui::SliderFloat3("Position", &global_lights[i].position.x, -20.0f, 20.0f);
        ImGui::ColorEdit3("Color", &global_lights[i].color.x);
        if (ImGui::Button("Remove Light")) {
            global_lights.erase(global_lights.begin() + i);
            ImGui::PopID();
            break;
        }
        ImGui::Separator();
        ImGui::PopID();
    }

    auto& settings = game->get_settings();
    auto& position = game->get_perlin_position();

    ImGui::SliderFloat2("Offset Position", &settings.x_offset, -100.0f, 100.0f);
    ImGui::InputInt("Seed", &settings.seed);

    int res = static_cast<int>(settings.meshResolution);
    if (ImGui::InputInt("Resolution", &res))
        settings.meshResolution = std::max(1, res);

    ImGui::SliderFloat("Scale", &settings.scale, 0.0f, 30.0f);
    ImGui::SliderFloat("Height Multiplier", &settings.heightMultiplier, 0.0f, 30.0f);
    ImGui::InputInt("Octaves", &settings.octaves);
    ImGui::SliderFloat("Persistence", &settings.persistence, 0.0f, 3.0f);
    ImGui::SliderFloat("Lacunarity", &settings.lacunarity, 0.0f, 3.0f);
    ImGui::SliderFloat3("Position", &position.x, -30.0f, 30.0f);

    if (ImGui::InputInt("Mesh Size", reinterpret_cast<int*>(&settings.sizeOfMesh))) {
        settings.sizeOfMesh = std::max(1u, settings.sizeOfMesh);
    }

    if (ImGui::Button("Add Light"))
        global_lights.emplace_back(glm::vec3(0.0f), glm::vec3(1.0f));

    ImGui::Selectable("Draw Normals", &game->should_draw_normals_geom, 0, ImVec2(90, 15));

    ImGui::End();

    ImGui::Begin("Editor");
    editor.Draw("My Bezier Curve", game->get_curve());

    ImGui::End();
}


void TestGUI::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
