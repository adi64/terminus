#pragma once

namespace terminus
{

class MathUtil
{
public:
    static const float PI;

public:
    static float clamp(float min, float max, float x);
    static float mix(float v0, float v1, float f);
    static float linstep(float e0, float e1, float x);
    static float smoothstep(float e0, float e1, float x);
    static float distance(float x0, float y0, float x1, float y1);
public:
    MathUtil();
    virtual ~MathUtil() = 0;
};

}//namespace terminus
