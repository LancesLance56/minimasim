#include "bezier.h"

float bezier(const float p0, const float p1, const float p2, const float p3, const float t) {
    const float u = 1.0f - t;
    const float tt = t* t;
    const float uu = u* u;
    const float uuu = uu* u;
    const float ttt = tt* t;

    return uuu* p0 + 3* uu* t* p1 + 3* u* tt* p2 + ttt* p3;
}
