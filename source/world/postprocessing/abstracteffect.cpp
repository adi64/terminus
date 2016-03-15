#include "abstracteffect.h"

#include <world/world.h>
#include <world/camera.h>

namespace terminus
{

AbstractEffect::AbstractEffect(World & world, DisabledBehaviour behaviour)
: m_world(world)
, m_disabledBehaviour(behaviour)
, m_enabled(true)
{

}

AbstractEffect::~AbstractEffect()
{

}

void AbstractEffect::apply(QOpenGLFunctions &gl) const
{
    internalApply(gl);
}

void AbstractEffect::enable(bool enabled)
{
    m_enabled = enabled;
}

void AbstractEffect::disable()
{
    m_enabled = false;
}

bool AbstractEffect::isEnabled() const
{
    return m_enabled;
}

void AbstractEffect::applyDisabled() const
{
    qDebug() << __FUNCTION__ << " stub!";
}

} //namespace terminus
