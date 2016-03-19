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
            //float angleZ = atan2(lookAt.y(), lookAt.x()) * 180 / MathUtil::PI;

            QQuaternion xz_rotation = QQuaternion::fromAxisAndAngle(QVector3D(0.0, 1.0, 0.0), angleY);
            QQuaternion y_rotationX = QQuaternion::fromAxisAndAngle(QVector3D(1.0, 0.0, 0.0), angleX);
    //        QQuaternion y_rotationZ = QQuaternion::fromAxisAndAngle(QVector3D(0.0, 0.0, 1.0), angleZ);

            setRotation(xz_rotation + y_rotationX);
        }
        else
        {
            QVector3D lookAt = parent()->modelMatrix() * QVector3D(1.f, 1.f, 1.f);

            float angleY = atan2(lookAt.z(), -lookAt.x()) * 180 / MathUtil::PI;
            float angleX = atan2(-lookAt.y(), lookAt.z()) * 180 / MathUtil::PI;

            QQuaternion xz_rotation = QQuaternion::fromAxisAndAngle(QVector3D(0.0, 1.0, 0.0), angleY);
            QQuaternion y_rotationX = QQuaternion::fromAxisAndAngle(QVector3D(1.0, 0.0, 0.0), angleX);

            setRotation(xz_rotation + y_rotationX);
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
