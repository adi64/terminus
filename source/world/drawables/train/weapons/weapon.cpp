#include "Weapon.h"

#include <cmath>

#include <resources/soundmanager.h>
#include <util/mathutil.h>

#include <world/camera.h>
#include <world/world.h>
#include <world/physics/kinematicphysicsobject.h>
#include <world/drawables/train/train.h>
#include <world/drawables/train/wagons/weaponwagon.h>
#include <world/drawables/train/weapons/turret.h>
#include <world/drawables/train/weapons/barrel.h>
#include <world/drawables/projectile.h>

#include <network/networkmanager.h>

#include <qDebug>

namespace terminus
{

Weapon::Weapon(World & world, WeaponWagon * parent)
 : AbstractGraphicsObject(world, parent)
 , m_damage(0.0)
 , m_reloadTime(0.0)
 , m_scattering(0.0)
 , m_thrust(0.0)
 , m_magazineSize(0)
 , m_turret(std::unique_ptr<Turret>(new Turret(world, this, "bigTurret_bigTurret", "base_Red")))
 , m_barrel(std::unique_ptr<Barrel>(new Barrel(world, this, "bigBarrel_bigBarrel", "base_Blue")))
{
    m_cameraTimer = m_world.timer().allocateTimer();
}

Weapon::~Weapon()
{
}

void Weapon::fire(QVector3D velocity, QVector3D position)
{
    m_world.scheduleAction(
        [this, position, velocity]()
        {
            auto projectile = new Projectile(m_world);
            projectile->moveTo(modelToWorld(position));
            projectile->setLinearVelocity(velocity);
            m_world.addObject(projectile);
            return false;
        }
    );

    m_world.networkManager().sendProjectileFiredCommand(position, velocity);

    SoundManager::getInstance()->playSound("shot");
}

void Weapon::localUpdate()
{
    AbstractGraphicsObject::localUpdate();

    //modelMatrix(); //muss das hier sein?

//        QVector3D lookAt = (m_camera->eye() - m_camera->center()).normalized();

//        float angleY = atan2(lookAt.z(), -lookAt.x()) * 180 / MathUtil::PI;
//        float angleX = atan2(-lookAt.y(), lookAt.z()) * 180 / MathUtil::PI;
//        float angleZ = atan2(lookAt.y(), lookAt.x()) * 180 / MathUtil::PI;

//        QQuaternion xz_rotation = QQuaternion::fromAxisAndAngle(QVector3D(0.0, 1.0, 0.0), angleY);
//        QQuaternion y_rotationX = QQuaternion::fromAxisAndAngle(QVector3D(1.0, 0.0, 0.0), angleX);
//        QQuaternion y_rotationZ = QQuaternion::fromAxisAndAngle(QVector3D(0.0, 0.0, 1.0), angleZ);

//        m_turret->setRotation(xz_rotation + y_rotationX);
     m_turret->localUpdate();

//        m_barrel->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.0, 1.0, 0.0), angleY));
     m_barrel->localUpdate();
}

QVector3D Weapon::weaponOffset()
{
    return QVector3D(0.0, 1.0, 2.0);
}

void Weapon::onBindCamera()
{
    if(!parent())
    {
        return;
    }

    m_cameraEyeOffset = QVector3D(0.f, 0.f, 0.f);
    m_previousCenter = worldToModel(m_camera->center());
    m_previousEye = worldToModel(m_camera->eye());
    m_world.timer().adjust(m_cameraTimer, 0);
}

void terminus::Weapon::adjustCamera()
{
    if(!m_camera)
    {
        return;
    }
    const int transitionTime = 200;
    float currentInfluence = MathUtil::linstep(0, transitionTime, m_world.timer().get(m_cameraTimer));

    auto vCenterM = MathUtil::mix(m_previousCenter, localCameraCenter(), currentInfluence);
    auto vEyeM = MathUtil::mix(m_previousEye, localCameraEye(), currentInfluence);

    m_camera->setCenter(modelToWorld(vCenterM));
    m_camera->setEye(modelToWorld(vEyeM));
}

void Weapon::rotateEvent(QVector2D rotation)
{
    if(!parent())
    {
        return;
    }

    auto scale = 0.025f;
    auto & vBBMinM = parent()->minBB();
    auto & vBBMaxM = parent()->maxBB();
    m_cameraEyeOffset.setX(
        MathUtil::clamp(vBBMinM.x(), vBBMaxM.x(), m_cameraEyeOffset.x() + rotation.x() * scale));
    m_cameraEyeOffset.setY(
        MathUtil::clamp(-1.f, 3.f, m_cameraEyeOffset.y() + rotation.y() * scale));
}

QVector3D Weapon::localCameraCenter()
{
    if(!parent())
    {
        return {0.f, 0.f, 0.f};
    }

    auto & vBBMinM = parent()->minBB();
    auto & vBBMaxM = parent()->maxBB();

    auto xCenterM = (vBBMinM.x() + vBBMaxM.x()) * 0.5f;
    auto yBaseM = vBBMaxM.y() + 1.f;

    if(dynamic_cast<WeaponWagon*>(parent())->isOtherTrainLeft())
    {
        return {xCenterM, yBaseM, vBBMinM.z() - 1.f};
    }
    else
    {
        return {xCenterM, yBaseM, vBBMaxM.z() + 1.f};
    }
}

QVector3D Weapon::localCameraEye()
{
    if(!parent())
    {
        return {0.f, 0.f, 0.f};
    }

    auto & vBBMinM = parent()->minBB();
    auto & vBBMaxM = parent()->maxBB();

    auto xCenterM = (vBBMinM.x() + vBBMaxM.x()) * 0.5f;
    auto yBaseM = vBBMaxM.y() + 1.f;
    auto & vEyeOff = m_cameraEyeOffset;

    if(dynamic_cast<WeaponWagon*>(parent())->isOtherTrainLeft())
    {
        return {xCenterM - vEyeOff.x(), yBaseM + vEyeOff.y(), vBBMaxM.z() + 2.f - vEyeOff.z()};
    }
    else
    {
        return {xCenterM + vEyeOff.x(), yBaseM + vEyeOff.y(), vBBMinM.z() - 2.f + vEyeOff.z()};
    }
}

Camera * Weapon::camera()
{
    return m_camera;
}

QMatrix4x4 Weapon::modelMatrix() const
{
    if(parent())
    {
        return parent()->modelMatrix();
    }
    return QMatrix4x4();
}

QMatrix4x4 Weapon::modelMatrixInverted() const
{
    if(parent())
    {
        return parent()->modelMatrixInverted();
    }
    return QMatrix4x4();
}

QVector3D Weapon::position() const
{
    if(parent())
    {
        return parent()->position();
    }
    return QVector3D(0.f, 0.f, 0.f);
}

float Weapon::damage()
{
    return m_damage;
}

float Weapon::reloadTime()
{
    return m_reloadTime;
}

float Weapon::scattering()
{
    return m_scattering;
}

float Weapon::thrust()
{
    return m_thrust;
}

int Weapon::magazineSize()
{
    return m_magazineSize;
}

void Weapon::setDamage(float amount)
{
    m_damage = amount;
}

void Weapon::setReloadTime(float amount)
{
    m_reloadTime = amount;
}

void Weapon::setScattering(float amount)
{
    m_scattering = amount;
}

void Weapon::setThrust(float amount)
{
    m_thrust = amount;
}

void Weapon::setMagazineSize(int amount)
{
    m_magazineSize = amount;
}

void terminus::Weapon::doForAllChildren(std::function<void (AbstractGraphicsObject &)> callback)
{
    callback(*m_turret);
    callback(*m_barrel);
}

bool terminus::Weapon::localRenderEnabled() const
{
    return false;
}

} //terminus
