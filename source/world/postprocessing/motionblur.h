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

    virtual unsigned int requiredNumberOfInputTextures() const override { return 2; }

    virtual void localRenderSetup(const Camera & camera, Program & program) const override;
    virtual void localRenderCleanup(const Camera & camera, Program & program) const override;

protected:
    mutable QMatrix4x4 m_previousViewProjectionMatrix;
    mutable bool m_initialized;
    int m_motionBlurFactor;
};

}
