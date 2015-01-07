#pragma once

#include "abstractwagon.h"

namespace terminus
{

class WeaponWagon : public AbstractWagon
{
public:
    WeaponWagon(Scene *scene, Train *train);

    void primaryAction() override;

    void render(QOpenGLFunctions& gl) const override;
    float length() const;
};

}
