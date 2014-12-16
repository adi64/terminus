#pragma once

#include "abstractwagon.h"

namespace terminus
{

class EngineWagon : public AbstractWagon
{
public:
    EngineWagon(Scene *scene, Train *train);
    void render(QOpenGLFunctions& gl, int elapsedMilliseconds);
    float length() const;
    void playSound();

    void shoot() override;
};

}
