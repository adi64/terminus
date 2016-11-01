#pragma once

#include <QString>
#include <memory>

#include <util/timer.h>
#include <world/physics/kinematicphysicsobject.h>

namespace terminus
{
class AbstractGraphicsObject;
class Train;

enum class WagonType
{
    INVALID = 0,
    ENGINE_WAGON = 1,
    WEAPON_WAGON = 2,
    REPAIR_WAGON = 3
};

/*!
 * \brief The AbstractWagon class provides a common interface for all wagon
 * types and contains common functionality like position calculation &c.
 */
class AbstractWagon : public KinematicPhysicsObject
{
public:
    AbstractWagon(World & world, AbstractGraphicsObject * parent, Train * train);
    virtual ~AbstractWagon();

    /*!
     * \brief Invokes wagon-specific primary action if the wagon is enabled
     * and is not on cooldown
     */
    virtual void primaryAction() final;

    /*!
     * \brief Invokes wagon-specific primary action without any checks
     */
    virtual void primaryActionDebug() final;

    /*!
     * \brief The method that every wagon should override in order to define
     * the wagon-specific primary action.
     */
    virtual void primaryActionInternal() = 0;

    /*!
     * \brief Updates wagon position and rotation
     */
    virtual void localUpdate() override;

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
    virtual void adjustCamera();

    /*!
     * \brief rotates the camera position in model space
     * \param rotation
     */
    virtual void rotateEvent(QVector2D rotation);

    virtual void setHealth(float health);
    virtual float currentHealth() const;
    virtual float maxHealth() const;

    virtual void resetCooldown() const;
    virtual float cooldown() const;
    virtual float cooldownTime() const = 0;
    virtual bool isOnCooldown() const;

    virtual bool isDisabled() const;
    virtual WagonType wagonType() const;
    virtual float length() const;

    /*!
     * \brief Determines whether the primary view direction should be left or
     * right
     */
    virtual bool isOtherTrainLeft() const;

    /*!
     * \brief Sets the distance between this wagon and the first wagon on the spline
     * \param accumulatedOffset The distance on the spline, in abstract units
     */
    virtual void setPositionOffset(float accumulatedOffset);

    /*!
     * \brief Shakes the camera if the wagon was hit by a Projectile
     * \param other The AbstractPhysicsObject that hit this wagon
     * \sa Projectile
     */
    virtual void onCollisionWith(AbstractPhysicsObject * other) override;

    virtual Train * train();

protected:
    virtual short myCollisionType() const override;
    virtual short possibleCollisionTypes() const override;

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

protected:
    QVector3D m_cameraEyeOffset;
    QVector3D m_previousEye;
    QVector3D m_previousCenter;
    Timer::TimerID m_cameraTimer;

    float m_positionOffset;
    float m_health;
    bool m_disabled;

    Timer::TimerID m_cooldownTimer;

    Train *m_train;
};

}
