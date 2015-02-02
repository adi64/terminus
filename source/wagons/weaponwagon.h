#pragma once

#include "abstractwagon.h"
#include "../timer.h"

namespace terminus
{

class WeaponWagon : public AbstractWagon
{
public:
    WeaponWagon(std::shared_ptr<Scene> scene, Train *train);
    virtual ~WeaponWagon();

    void primaryAction() override;
    void primaryActionDebug() override;

    void setChargeProjectile(bool charge);

    void update() override;
    void preRender(QOpenGLFunctions& gl, Program & program) const override;
    float length() const;

protected:
    void fire(QVector3D force);

protected:
    Timer::TimerID m_reloadTimer;
    bool m_chargeProjectile;
    bool m_reloadProjectile;
    float m_force;
};

}
