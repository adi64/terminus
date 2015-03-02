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

    float cooldownRate() const override;

    WagonType wagonType() const override;
    float length() const override;

    void localUpdate() override;
    void localRenderSetup(QOpenGLFunctions& gl, Program & program) const override;

protected:
    /*!
     * \brief Spawn a projectile and set velocity
     * \param velocity Velocity vector in global coordinates (will not be rotated to match object's rotation)
     *
     * Schedules a projectile spawn (for next frame) with some position offset, copies the current velocity to the projectile and applies the given velocity to it.
     */
    void fire(QVector3D velocity);

protected:
    QVector3D m_normalizedAimVector;
};

}
