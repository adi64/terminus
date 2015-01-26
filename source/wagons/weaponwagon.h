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

    void setAimVector(const QVector3D& aimVector);

    void fire(QVector3D force);

    bool isReloading() const;

    void update(int elapsedMilliseconds) override;
    void render(QOpenGLFunctions& gl) const override;
    float length() const;

protected:
    int maxChargeMilliseconds() const;
    QVector3D getAimDeviation() const;
protected:
    int m_elapsedMilliseconds;
    bool m_chargeProjectile;
    bool m_reloadProjectile;
    QVector3D m_normalizedAimVector;
};

}
