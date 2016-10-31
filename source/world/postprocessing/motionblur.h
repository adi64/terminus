#pragma once

#include <QMatrix4x4>

#include <world/postprocessing/abstracteffect.h>

namespace terminus
{

class MotionBlur : public AbstractEffect
{
public:
    MotionBlur(const World & world, DisabledBehaviour behaviour = DisabledBehaviour::PASS);
    virtual ~MotionBlur();

    virtual void localRenderSetup(Program & program) const override;
    virtual void localRenderCleanup(Program & program) const override;

protected:
    QMatrix4x4 m_previousViewProjectionMatrix;

    float m_motionBlurFactor;
};

}
