#include "projectile.h"

#include <QDebug>


#include <resources/resourcemanager.h>
#include <resources/geometry.h>
#include <resources/material.h>
#include <resources/program.h>
#include <util/timer.h>
#include <world/drawables/train/wagons/abstractwagon.h>
#include <world/world.h>

namespace terminus
{

Projectile::Projectile(World & world)
: DynamicPhysicsObject(world)
{   
    m_program = ResourceManager::getInstance()->getProgram("basicShader");
    m_geometry = ResourceManager::getInstance()->getGeometry("base_Icosahedron");
    m_material = ResourceManager::getInstance()->getMaterial("base_Red");

    m_lifeTimer = m_world.timer().allocateTimer();

    initializePhysics(new btSphereShape(1.0), 1.f);
}

Projectile::~Projectile()
{
    deallocatePhysics();

    m_world.timer().releaseTimer(m_lifeTimer);
}

void Projectile::localUpdate()
{
    DynamicPhysicsObject::localUpdate();

    if(m_world.timer().get(m_lifeTimer) > maxAgeInMilliseconds())
    {
        m_world.scheduleAction(
            [this]()
            {
                m_world.deleteNode(this);
                delete(this);
                return false;
            });
    }
}

void Projectile::localRenderSetup(QOpenGLFunctions & gl, Program & program) const
{
    program.setUniform(std::string("lightDirection"), QVector3D(100.0, 20.0, -100.0));
}

float Projectile::damage() const
{
    return 30.0f;
}

void Projectile::onCollisionWith(AbstractPhysicsObject *other)
{
    auto otherWagon = dynamic_cast<AbstractWagon*>(other);
    if(otherWagon)
    {
        otherWagon->setHealth(otherWagon->currentHealth() - damage());
    }
}

unsigned int Projectile::maxAgeInMilliseconds() const
{
    return 5000;
}

short Projectile::myCollisionType() const
{
    return BulletWorld::CollisionTypes::COLLISIONTYPE_PROJECTILE;
}

short Projectile::possibleCollisionTypes() const
{
    return BulletWorld::CollisionTypes::COLLISIONTYPE_WAGON;
}

} //namespace terminus
