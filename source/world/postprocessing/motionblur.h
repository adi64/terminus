#pragma once

#include <QMatrix4x4>

#include <world/postprocessing/abstracteffect.h>

namespace terminus
{

class MotionBlur : public AbstractEffect
{
public:
    MotionBlur();
    virtual ~MotionBlur(DisabledBehaviour behaviour = DisabledBehaviour::PASS);

    virtual void internalApply(QOpenGLFunctions & gl) const override;

protected:
    QMatrix4x4 m_previousViewProjectionMatrix;
};

}
