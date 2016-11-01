#pragma once

#include <world/physics/kinematicphysicsobject.h>

#include <util/timer.h>

namespace terminus
{

class WeaponWagon;
class Turret;
class Barrel;

class Weapon : public AbstractGraphicsObject
{

public:
    Weapon(World & world, WeaponWagon * parent);
    virtual ~Weapon();

public:
    void fire(QVector3D velocity, QVector3D position);
    virtual void localUpdate() override;

    //TODO: What purpose?
    QVector3D weaponOffset();

    /*!
     * \brief saves the previous camera position to animate the
     * camera transition
     */
    virtual void onBindCamera() override;

    /*!
     * \brief moves the camera according to this wagons position and orientation
     *
     * \sa localCameraCenter()
     * \sa localCameraEye()
     */
    virtual void adjustCamera() override;

    /*!
     * \brief rotates the camera position in model space
     * \param rotation
     */
    virtual void rotateEvent(QVector2D rotation) override;

    /*!
     * \brief Calculates the camera position vector in model space.
     * \return
     */
    virtual QVector3D localCameraCenter();

    /*!
     * \brief Calculates the camera eye vector in model space.
     * \return
     */
    virtual QVector3D localCameraEye();

public:
    float damage();
    float reloadTime();
    float scattering();
    float thrust();
    int magazineSize();

    void setDamage(float amount);
    void setReloadTime(float amount);
    void setScattering(float amount);
    void setThrust(float amount);
    void setMagazineSize(int amount);

protected:
    virtual void doForAllChildren(std::function<void(AbstractGraphicsObject &)> callback) override;
    virtual bool localRenderEnabled() const override;

protected:
    float m_damage;
    float m_reloadTime;
    float m_scattering;
    float m_thrust;
    int m_magazineSize;

    QVector3D m_cameraEyeOffset;
    QVector3D m_previousEye;
    QVector3D m_previousCenter;
    Timer::TimerID m_cameraTimer;

    std::unique_ptr<Turret> m_turret;
    std::unique_ptr<Barrel> m_barrel;

};

} //namespace
