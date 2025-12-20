#ifndef ANIMATION_CURVE_H
#define ANIMATION_CURVE_H

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <vector>

struct BezierKeyframe {
    float time = 0.0f;
    float value = 0.0f;
    ImVec2 tangentOffset = ImVec2(30, 0); // default handle offset in pixels
    bool draggingPoint = false;
    bool draggingTangent = false;
};

inline bool operator==(const std::vector<BezierKeyframe> &a, const std::vector<BezierKeyframe> &b) {
    if (a.size() != b.size())
        return false;

    for (int i = 0; i < a.size(); i++) {
        if (a[i].time != b[i].time || a[i].value != b[i].value || a[i].tangentOffset.x != b[i].tangentOffset.x ||
            a[i].tangentOffset.y != b[i].tangentOffset.y || a[i].draggingPoint != b[i].draggingPoint ||
            a[i].draggingTangent != b[i].draggingTangent) {
            return false;
        }
    }

    return true;
}


using BezierCurve = std::vector<BezierKeyframe>;

class BezierEditor {
public:
    bool isAPointSelected = false;
    BezierEditor() = default;
    explicit BezierEditor(GLFWwindow* glfwWindow);
    static ImVec2 ToScreen(ImVec2 origin, ImVec2 size, float t, float v);
    static ImVec2 FromScreen(ImVec2 origin, ImVec2 size, ImVec2 screen);

    // Call every frame inside ImGui::Begin/End
    void Draw(const char* label, BezierCurve &curve, ImVec2 canvasSize = ImVec2(500, 300));
    static std::vector<float> computeHeightRemapLUT(const std::vector<BezierKeyframe> &keyframes, int resolution = 256);

private:
    GLFWwindow* m_window{};

    // Operator helpers
    [[nodiscard]] ImVec2 MousePos() const;
};

#endif
