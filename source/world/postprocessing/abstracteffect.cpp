#include "abstracteffect.h"

#include <world/game.h>
#include <player/camera.h>

namespace terminus
{

AbstractEffect::AbstractEffect(Game & world, DisabledBehaviour behaviour)
: GameObject(world)
, m_disabledBehaviour(behaviour)
, m_enabled(true)
{
    m_geometry = ResourceManager::getInstance()->getGeometry("base_squad");
}

AbstractEffect::~AbstractEffect()
{

}

void AbstractEffect::localRenderSetup(Program & /*program*/) const
{
}
void AbstractEffect::localRenderCleanup(Program & /*program*/) const
{
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
