#pragma once

#include "abstractwagon.h"

namespace terminus
{

class WeaponWagon : public AbstractWagon
{
public:
    WeaponWagon(World & world, Train *train);
    virtual ~WeaponWagon();

    void primaryAction() override;
    void primaryActionDebug() override;

    void setAimVector(const QVector3D& aimVector);

    bool isReloading() const;

    void localUpdate() override;
    void localRenderSetup(QOpenGLFunctions& gl, Program & program) const override;
    float length() const override;
    WagonType wagonType() const override;
    unsigned int reloadTime() const;

protected:
    /*!
     * \brief Spawn a projectile and apply force
     * \param force Force vector in global coordinates (will not be rotated to match object's rotation)
     *
     * Schedules a projectile spawn (for next frame) with some position offset, copies the current velocity to the projectile and applies the given force to it.
     */
    void fire(QVector3D force);

protected:
    bool m_reloadProjectile;
    unsigned int m_reloadTime;
    QVector3D m_normalizedAimVector;
};

}
