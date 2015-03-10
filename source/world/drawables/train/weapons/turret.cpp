#include "turret.h"

#include <QDebug>

#include <resources/resourcemanager.h>
#include <world/drawables/abstractgraphicsobject.h>

namespace terminus
{

Turret::Turret(World & world, std::string geometry, std::string material)
 : KinematicPhysicsObject(world)
{
    m_program = ResourceManager::getInstance()->getProgram("basicShader");
    m_geometry = ResourceManager::getInstance()->getGeometry(geometry);
    m_material = ResourceManager::getInstance()->getMaterial(material);

    initializePhysics(new btSphereShape(1.0), 1.f);
}

Turret::~Turret()
{
}

void Turret::localUpdate()
{
//    setPosition(position);
//    m_scale = QVector3D(position.x(), position.x(), position.x())/10;

//    qDebug() << __FILE__ << __LINE__;

//    KinematicPhysicsObject::localUpdate();
}

short Turret::myCollisionType() const
{
    return BulletWorld::CollisionTypes::COLLISIONTYPE_WAGON;
}

short Turret::possibleCollisionTypes() const
{
    return BulletWorld::CollisionTypes::COLLISIONTYPE_PROJECTILE;
}

} //terminus
