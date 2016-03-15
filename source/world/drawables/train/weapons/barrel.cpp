#include "barrel.h"

#include <util/mathutil.h>

#include <resources/resourcemanager.h>

#include <world/camera.h>
#include <world/drawables/train/weapons/weapon.h>

namespace terminus
{

Barrel::Barrel(World & world, Weapon * parent, std::string geometry, std::string material)
 : KinematicPhysicsObject(world, parent)
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
    if(m_parent)
    {
        Camera camera = *dynamic_cast<Weapon*>(parent())->camera();
        QVector3D lookAt = (camera.eye() - camera.center()).normalized();

        float angleY = atan2(lookAt.z(), -lookAt.x()) * 180 / MathUtil::PI;
        //float angleX = atan2(-lookAt.y(), lookAt.z()) * 180 / MathUtil::PI;
        //float angleZ = atan2(lookAt.y(), lookAt.x()) * 180 / MathUtil::PI;

//        QQuaternion xz_rotation = QQuaternion::fromAxisAndAngle(QVector3D(0.0, 1.0, 0.0), angleY);
//        QQuaternion y_rotationX = QQuaternion::fromAxisAndAngle(QVector3D(1.0, 0.0, 0.0), angleX);
//        QQuaternion y_rotationZ = QQuaternion::fromAxisAndAngle(QVector3D(0.0, 0.0, 1.0), angleZ);

        setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.0, 1.0, 0.0), angleY));
    }

    KinematicPhysicsObject::localUpdate();
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