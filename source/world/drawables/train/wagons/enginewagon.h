#pragma once

#include "abstractwagon.h"

namespace terminus
{

class EngineWagon : public AbstractWagon
{


public:
    EngineWagon(World & world, Train * train);
    virtual ~EngineWagon();

    void localUpdate() override;
    void localRenderSetup(QOpenGLFunctions& gl, Program & m_program) const override;

    float length() const override;
    WagonType wagonType() const override;

    void playSound() const;
};

}
