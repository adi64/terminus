#include "barrel.h"

#include <QDebug>

#include <resources/resourcemanager.h>

namespace terminus
{

Barrel::Barrel(World & world, std::string geometry, std::string material)
 : KinematicPhysicsObject(world)
{
    m_program = ResourceManager::getInstance()->getProgram("basicShader");
    m_geometry = ResourceManager::getInstance()->getGeometry(geometry);
    m_material = ResourceManager::getInstance()->getMaterial(material);

    initializePhysics(new btSphereShape(1.0), 1.f);
}

Barrel::~Barrel()
{
}

void Barrel::localUpdate()
{
//    setPosition(position);

//        qDebug() << __FILE__ << __PRETTY_FUNCTION__;

//    KinematicPhysicsObject::localUpdate();
}

short Barrel::myCollisionType() const
{
    return BulletWorld::CollisionTypes::COLLISIONTYPE_WAGON;
}

short Barrel::possibleCollisionTypes() const
{
    return BulletWorld::CollisionTypes::COLLISIONTYPE_PROJECTILE;
}

} //terminus
