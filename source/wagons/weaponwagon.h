#pragma once

#include "abstractwagon.h"

namespace terminus
{

class Shot;

class WeaponWagon : public AbstractWagon
{
public:
    WeaponWagon(Scene *scene, Train *train);
    void render(QOpenGLFunctions& gl, int elapsedMilliseconds);
    float length() const;
    void shoot() override;

protected:
    std::vector<Shot*> m_shots;
};

}
