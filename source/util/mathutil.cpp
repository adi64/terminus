#include "mathutil.h"

#include <math.h>

namespace terminus
{
const float MathUtil::PI = atan(1.0f) * 4.0f;

float MathUtil::mix(float v0, float v1, float f)
{
    return (1.f - f) * v0 + f * v1;
}

float MathUtil::deg2rad(float degrees)
{
    return degrees * PI / 180.f;
}

float MathUtil::rad2deg(float radians)
{
    return radians * 180.f / PI;
}

float MathUtil::cosDeg(float degrees)
{
    return cos(deg2rad(degrees));
}

float MathUtil::clamp(float xMin, float xMax, float x)
{
    return fmin(fmax(x, xMin), xMax);
}
float MathUtil::linstep(float e0, float e1, float x)
{
    return clamp(0.f, 1.f, (x - e0)/(e1 - e0));
}

float MathUtil::smoothstep(float e0, float e1, float x)
{
    x = linstep(e0, e1, x);
    return x*x*x*(x*(x*6.f - 15.f) + 10.f);
}

float MathUtil::distance(float x0, float y0, float x1, float y1)
{
    float dx = x1 - x0, dy = y1 - y0;
    return sqrt(dx * dx + dy * dy);
}

MathUtil::MathUtil()
{
}
}

