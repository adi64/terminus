#pragma once

#include "abstractwagon.h"

namespace terminus
{

class EngineWagon : public AbstractWagon
{


public:
    EngineWagon(World & world, Train * train);
    virtual ~EngineWagon();

    void preRender(QOpenGLFunctions& gl, Program & m_program) const override;

    float length() const;

    void playSound() const;
};

}
