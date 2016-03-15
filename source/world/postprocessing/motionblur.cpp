#include "motionblur.h"

namespace terminus
{

MotionBlur::MotionBlur()
{

}

MotionBlur::~MotionBlur(DisabledBehaviour behaviour)
: AbstractEffect(behaviour)
{

}

void MotionBlur::internalApply(QOpenGLFunctions &gl) const
{

}

} //namespace terminus
