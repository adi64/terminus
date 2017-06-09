#include "explosion.h"

#include <QVector3D>

#include <resources/resourcemanager.h>
#include <resources/geometry.h>
#include <resources/program.h>
#include <util/mathutil.h>
#include <world/game.h>

namespace terminus
{

Explosion::Explosion(Game & world, const QVector3D & position)
: GameObject(world)
{
    m_program = ResourceManager::getInstance()->getProgram("basicShader");
    m_geometry = ResourceManager::getInstance()->getGeometry("base_ico2");
    m_material = ResourceManager::getInstance()->getMaterial("base_OrangeHalf");

    matrix().setPosition(position);

    m_timer = m_game.timer().allocateTimer();
}

Explosion::~Explosion()
{
    m_game.timer().releaseTimer(m_timer);
}

void Explosion::localUpdate()
{
    const int lifetimeMs = 1000;
    int timeMs = m_game.timer().get(m_timer);

    matrix().setScale(MathUtil::mix(3.f, 10.f, MathUtil::linstep(0, lifetimeMs, timeMs)));

    GameObject::localUpdate();

    if(timeMs > lifetimeMs)
    {
       dispose();
    }
}

void Explosion::localRenderSetup(Program &) const
{
    glDisable(GL_CULL_FACE);
}

void Explosion::localRenderCleanup(Program &) const
{
    glEnable(GL_CULL_FACE);
}

} //namespace terminus

