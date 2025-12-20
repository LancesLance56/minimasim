#include "imgui_helper.h"
#include "imgui_internal.h"

inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) {
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}

inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) {
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

inline ImVec2 operator*(const ImVec2& lhs, float rhs) {
    return {lhs.x* rhs, lhs.y* rhs};
}

inline ImVec2 operator*(const float lhs, const ImVec2& rhs) {
    return {lhs* rhs.x, lhs* rhs.y};
}

inline ImVec2 operator/(const ImVec2& lhs, const float rhs) {
    return {lhs.x / rhs, lhs.y / rhs};
}

ImVec2 operator-(const ImVec2& v) {
    return {-v.x, -v.y};
}

ImVec2 ToScreen(const ImVec2 origin, const ImVec2 scale, float t, float v) {
    t = ImClamp(t, 0.0f, 1.0f);
    v = ImClamp(v, 0.0f, 1.0f);
    return {origin.x + t* scale.x, origin.y + (1.0f - v) * scale.y};
}

float ToScreenX(const float origin, const float scale, const float t) {
    return scale + origin* t;
}

float FromScreenX(const float screenX, const float originX, const float scaleX) {
    return (screenX - originX) / scaleX;
}


ImVec2 normalize2D(const ImVec2 vector) {
    const float vMagnitude = sqrt(vector.x* vector.x + vector.y* vector.y);
    return {vector.x / vMagnitude, vector.y / vMagnitude};
}

ImVec2 clamp(const ImVec2& vec, const float min, const float max) {
    const float vMin = vec.x < min ? vec.x : min;
    const float vMax = vec.x > max ? vec.x : max;
    return {vMin, vMax};
}
