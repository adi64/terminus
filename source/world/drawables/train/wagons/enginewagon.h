#pragma once

#include "abstractwagon.h"

#include <resources/lightmanager.h>

namespace terminus
{

class EngineWagon : public AbstractWagon
{


public:
    EngineWagon(World & world, Train * train);
    virtual ~EngineWagon();

    virtual void localUpdate(int elapsedMilliseconds) override;
    virtual void localRenderSetup(QOpenGLFunctions & gl, Program & m_program) const override;

    float length() const;

    void playSound() const;

protected:
    LightID m_headLight;
};

}
