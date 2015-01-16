#pragma once

#include "abstractwagon.h"

namespace terminus
{

class EngineWagon : public AbstractWagon
{
public:
    EngineWagon(std::shared_ptr<Scene> scene, Train *train);
    void render(QOpenGLFunctions& gl) const override;
    float length() const;
    void playSound() const;
};

}
