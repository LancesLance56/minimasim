#include "animation_curve.h"

#include <algorithm>
#include <iostream>

// Removed "glm/detail/func_common.hpp" if it's not strictly needed for other parts of BezierEditor
// ImGui includes are handled via imgui_helper.h and imgui_internal.h
#include "glm/gtc/quaternion.hpp"
#include "imgui_helper.h"
#include "imgui_internal.h"

BezierEditor::BezierEditor(GLFWwindow* glfwWindow) : m_window(glfwWindow) {}

ImVec2 BezierEditor::ToScreen(const ImVec2 origin, const ImVec2 size, float t, float v) {
    t = ImClamp(t, 0.0f, 1.0f);
    v = ImClamp(v, 0.0f, 1.0f);
    return {origin.x + t* size.x, origin.y + (1.0f - v) * size.y};
}

ImVec2 BezierEditor::FromScreen(const ImVec2 origin, const ImVec2 size, const ImVec2 screen) {
    const float time = (screen.x - origin.x) / size.x;
    const float value = 1.0f - (screen.y - origin.y) / size.y;
    return {time, value};
}

ImVec2 BezierEditor::MousePos() const {
    double mx;
    double my;
    glfwGetCursorPos(m_window, &mx, &my);
    return {static_cast<float>(mx), static_cast<float>(my)};
}

void BezierEditor::Draw(const char* label, BezierCurve &curve, const ImVec2 canvasSize) {
    std::ranges::sort(curve, [](const BezierKeyframe& a, const BezierKeyframe& b) {
        return a.time < b.time;
    });

    ImGui::TextUnformatted(label);
    const ImVec2 origin = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    constexpr float margin = 20.0f;
    const ImVec2 inner_origin = origin + ImVec2(margin, margin);
    const ImVec2 inner_size = canvasSize - ImVec2(margin* 2, margin* 2);

    draw_list->AddRectFilled(origin, origin + canvasSize, IM_COL32(30, 30, 30, 255));
    draw_list->AddRect(origin, origin + canvasSize, IM_COL32(255, 255, 255, 100));
    ImGui::InvisibleButton("curve_canvas", canvasSize);

    for (int i = 0; i <= 10; ++i) {
        const float screen_x = inner_origin.x + static_cast<float>(i) / 10.0f* inner_size.x;
        const float screen_y = inner_origin.y + static_cast<float>(i) / 10.0f* inner_size.y;
        draw_list->AddLine(
                ImVec2(screen_x, inner_origin.y), ImVec2(screen_x, inner_origin.y + inner_size.y),
                IM_COL32(255, 255, 255, 20));
        draw_list->AddLine(
                ImVec2(inner_origin.x, screen_y), ImVec2(inner_origin.x + inner_size.x, screen_y),
                IM_COL32(255, 255, 255, 20));
    }

    const ImVec2 mouse = MousePos();
    const int mouseLeft = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT);

    // Tangent dragging and drawing (using indexed loop for next keyframe access)
    for (size_t i = 0; i < curve.size(); ++i) {
        // Use references to make code cleaner and directly modify members
        auto &[time, value, tangentOffset, draggingPoint, draggingTangent] = curve[i];

        ImVec2 p = ToScreen(inner_origin, inner_size, time, value);

        ImVec2 out_handle = p + tangentOffset;
        ImVec2 in_handle = p - tangentOffset;

        // Draw tangent lines
        draw_list->AddLine(p, out_handle, IM_COL32(100, 200, 255, 128));
        draw_list->AddLine(p, in_handle, IM_COL32(100, 200, 255, 128));

        // Draw tangent handles
        draw_list->AddCircleFilled(out_handle, 5.0f, IM_COL32(200, 100, 255, 255));
        draw_list->AddCircleFilled(in_handle, 5.0f, IM_COL32(200, 100, 255, 255));

        // Handle interaction for starting a drag
        if (!draggingPoint && !isAPointSelected) {
            if (ImLengthSqr(mouse - out_handle) < 25.0f && mouseLeft == GLFW_PRESS) {
                draggingTangent = true;
                isAPointSelected = true;
            }
            if (ImLengthSqr(mouse - in_handle) < 25.0f && mouseLeft == GLFW_PRESS) {
                draggingTangent = true;
                isAPointSelected = true;
            }
        }

        // Handle continuous dragging for tangent
        if (draggingTangent) {
            ImVec2 offset = mouse - p;
            if (ImLengthSqr(mouse - in_handle) < ImLengthSqr(mouse - out_handle)) {
                tangentOffset = -offset;
            } else {
                tangentOffset = offset;
            }
        }

        // --- REVISED TANGENT X-CLAMING LOGIC ---
        // Ensure the outgoing tangent of the current keyframe (k0) does not extend
        // horizontally past the next keyframe's position (k1.time).
        // This prevents the Bézier curve segment from "folding back" visually.
        if (i + 1 < curve.size()) {
            const BezierKeyframe& k1 = curve[i + 1]; // Reference to the next keyframe

            const float current_out_handle_screen_x = p.x + tangentOffset.x;

            const ImVec2 p1_screen = ToScreen(inner_origin, inner_size, k1.time, k1.value);

            if (const float next_keyframe_screen_x = p1_screen.x;
                current_out_handle_screen_x > next_keyframe_screen_x) {
                tangentOffset.x -= (current_out_handle_screen_x - next_keyframe_screen_x);
            }
        }

        tangentOffset.x = std::max(tangentOffset.x, 0.0f);
    }

    for (size_t i = 0; i + 1 < curve.size(); ++i) {
        const auto &k0 = curve[i];
        const auto &k1 = curve[i + 1];

        ImVec2 p0 = ToScreen(inner_origin, inner_size, k0.time, k0.value);
        ImVec2 p3 = ToScreen(inner_origin, inner_size, k1.time, k1.value);
        ImVec2 handle0 = p0 + k0.tangentOffset;
        ImVec2 handle1 = p3 - k1.tangentOffset;

        draw_list->AddBezierCubic(p0, handle0, handle1, p3, IM_COL32(100, 255, 100, 255), 2.0f);
        draw_list->AddLine(p0, handle0, IM_COL32(255, 200, 0, 128));
        draw_list->AddLine(p3, handle1, IM_COL32(255, 200, 0, 128));
    }

    // Draw and interact with keyframes (using range-based for loop as no next keyframe access needed)
    for (auto &k: curve) {
        constexpr float radius = 6.0f;
        ImVec2 p = ToScreen(inner_origin, inner_size, k.time, k.value);

        // Interaction for starting a drag
        if (const float distSq = ImLengthSqr(mouse - p);
            mouseLeft == GLFW_PRESS && distSq < radius* radius* 4 && !k.draggingTangent && !isAPointSelected) {
            isAPointSelected = true;
            k.draggingPoint = true;
        }

        // Handle continuous dragging for keyframe
        if (k.draggingPoint) {
            const ImVec2 delta = mouse - p;
            k.time += delta.x / inner_size.x;
            k.value -= delta.y / inner_size.y;
            k.time = ImClamp(k.time, 0.0f, 1.0f);
            k.value = ImClamp(k.value, 0.0f, 1.0f);
        }

        draw_list->AddCircleFilled(p, radius, k.draggingPoint ? IM_COL32(255, 255, 0, 255) : IM_COL32(255, 0, 0, 255));
    }

    // Global release check for all dragging states
    if (mouseLeft == GLFW_RELEASE) {
        for (auto &k: curve) {
            k.draggingPoint = false;
            k.draggingTangent = false;
        }
        isAPointSelected = false; // Reset global selection flag
    }
}

static float evaluateCubicBezier(float p0, float p1, float p2, float p3, float t) {
    float u = 1.0f - t;
    float tt = t* t;
    float uu = u* u;
    float uuu = uu* u;
    float ttt = tt* t;

    return uuu* p0 + 3.0f* uu* t* p1 + 3.0f* u* tt* p2 + ttt* p3;
}

std::vector<float> BezierEditor::computeHeightRemapLUT(const std::vector<BezierKeyframe> &keyframes, int resolution) {
    std::vector<float> lut(resolution);
    if (keyframes.size() < 2) return lut;

    for (int i = 0; i < resolution; ++i) {
        float t = static_cast<float>(i) / (resolution - 1); // global time in [0, 1]

        for (size_t j = 0; j < keyframes.size() - 1; ++j) {
            const auto &k0 = keyframes[j];
            const auto &k1 = keyframes[j + 1];

            if (t >= k0.time && t <= k1.time) {
                // Normalize to local segment [0, 1]
                float segmentDuration = k1.time - k0.time;
                float localT = (t - k0.time) / segmentDuration;

                float v0 = k0.value;
                float v3 = k1.value;

                float outTangentY = glm::normalize(glm::vec2(k0.tangentOffset.x, k0.tangentOffset.y)).y;
                float inTangentY = glm::normalize(glm::vec2(k1.tangentOffset.x, k1.tangentOffset.y)).y;

                float p1 = v0 + outTangentY;
                float p2 = v3 - inTangentY;

                lut[i] = evaluateCubicBezier(v0, p1, p2, v3, localT);
                break;
            }
        }
    }

    return lut;
}
