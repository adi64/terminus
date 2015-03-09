#include "explosion.h"

#include <QVector3D>

#include <world/world.h>
#include <resources/resourcemanager.h>
#include <resources/geometry.h>
#include <resources/program.h>
#include <util/mathutil.h>

namespace terminus
{

Explosion::Explosion(World & world, const QVector3D & position)
: AbstractGraphicsObject(world)
{
    m_program = ResourceManager::getInstance()->getProgram("basicShader");
    m_geometry = ResourceManager::getInstance()->getGeometry("base_ico2");
    m_material = ResourceManager::getInstance()->getMaterial("base_OrangeHalf");

    setPosition(position);

    m_timer = m_world.timer().allocateTimer();
}

Explosion::~Explosion()
{
    m_world.timer().releaseTimer(m_timer);
}

void Explosion::localUpdate()
{
    const int lifetimeMs = 1000;
    int timeMs = m_world.timer().get(m_timer);

    setScale(MathUtil::mix(3.f, 10.f, MathUtil::linstep(0, lifetimeMs, timeMs)));

    AbstractGraphicsObject::localUpdate();

    if(timeMs > lifetimeMs)
    {
       dispose();
    }
}

void Explosion::localRenderSetup(QOpenGLFunctions & gl, Program &) const
{
    gl.glDisable(GL_CULL_FACE);
}

void Explosion::localRenderCleanup(QOpenGLFunctions & gl, Program &) const
{
    gl.glEnable(GL_CULL_FACE);
}

}

