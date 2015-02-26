#pragma once

#include "abstractwagon.h"

namespace terminus
{

class EngineWagon : public AbstractWagon
{


public:
    EngineWagon(std::shared_ptr<Scene> scene, Train *train);
    virtual ~EngineWagon();

    void preRender(QOpenGLFunctions& gl, Program & m_program) const override;

    float length() const override;
    WagonType wagonType() const override;

    void playSound() const;
};

}
