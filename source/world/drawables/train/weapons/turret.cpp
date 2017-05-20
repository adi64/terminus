#include "turret.h"

#include <QDebug>

#include <util/mathutil.h>

#include <world/camera.h>

#include <resources/resourcemanager.h>
#include <world/drawables/abstractgraphicsobject.h>
#include <world/drawables/train/weapons/weapon.h>

namespace terminus
{

Turret::Turret(World & world, Weapon * parent, std::string geometry, std::string material)
 : KinematicPhysicsObject(world, parent)
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
    if(parent())
    {
        if(dynamic_cast<Weapon*>(parent())->camera())
        {
            Camera camera = *dynamic_cast<Weapon*>(parent())->camera();

            QVector3D lookAt = (camera.eye() - camera.center()).normalized();
            float angleY = atan2(lookAt.z(), -lookAt.x()) * 180 / MathUtil::PI;
            float angleX = atan2(-lookAt.y(), lookAt.z()) * 180 / MathUtil::PI;            
            QQuaternion y_rotation = QQuaternion::fromAxisAndAngle(QVector3D(0.0, 1.0, 0.0), angleY+90);

            setRotation(y_rotation);
            setPosition(QVector3D(parent()->position().x() + (61.46 * 0.0015), parent()->position().y() + (764.384 * 0.0015), parent()->position().z() + (44.331 * 0.0015)));
        }
        else
        {
            QVector3D lookAt = parent()->modelMatrix() * QVector3D(1.f, 1.f, 1.f);
            float angleY = atan2(lookAt.z(), -lookAt.x()) * 180 / MathUtil::PI;
            float angleX = atan2(-lookAt.y(), lookAt.z()) * 180 / MathUtil::PI;
            QQuaternion y_rotationX = QQuaternion::fromAxisAndAngle(QVector3D(1.0, 0.0, 0.0), angleX);

            setRotation(y_rotationX);
            //qDebug() << position() << "\n";
            setPosition(QVector3D(parent()->position().x() + (61.46 * 0.0015), parent()->position().y() + (764.384 * 0.0015), parent()->position().z() + (44.331 * 0.0015)));
        }
    }

    KinematicPhysicsObject::localUpdate();
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
