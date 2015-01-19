#pragma once

#include "abstractwagon.h"

namespace terminus
{

class WeaponWagon : public AbstractWagon
{
public:
    WeaponWagon(std::shared_ptr<Scene> scene, Train *train);

    void primaryAction() override;

    void preRender(QOpenGLFunctions& gl, Program & program) const override;
    float length() const;
};

}
