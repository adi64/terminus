#pragma once

#include "abstractwagon.h"

namespace terminus
{

class WeaponWagon : public AbstractWagon
{
public:
    WeaponWagon(std::shared_ptr<Scene> scene, Train *train);

    void primaryAction() override;
    void primaryActionDebug() override;

    void setChargeProjectile(bool charge);

    void fire(QVector3D force);

    void update(int elapsedMilliseconds) override;
    void render(QOpenGLFunctions& gl) const override;
    float length() const;
    unsigned int chargeTime() const;
    unsigned int reloadTime() const;

protected:
    bool m_chargeProjectile;
    unsigned int m_chargeTime;
    bool m_reloadProjectile;
    unsigned int m_reloadTime;
    float m_force;
};

}
