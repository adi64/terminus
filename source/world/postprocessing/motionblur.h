#pragma once

#include <QMatrix4x4>

#include <world/postprocessing/abstracteffect.h>

namespace terminus
{

class MotionBlur : public AbstractEffect
{
public:
    MotionBlur(World & world, DisabledBehaviour behaviour = DisabledBehaviour::PASS);
    virtual ~MotionBlur();

    virtual void localRenderSetup(QOpenGLFunctions & gl, Program & program) const override;
    virtual void localRenderCleanup(QOpenGLFunctions & gl, Program & program) const override;

protected:
    QMatrix4x4 m_previousViewProjectionMatrix;

    float m_motionBlurFactor;
};

}
