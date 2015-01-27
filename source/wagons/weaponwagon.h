#pragma once

#include "abstractwagon.h"

namespace terminus
{

class WeaponWagon : public AbstractWagon
{
public:
    WeaponWagon(std::shared_ptr<Scene> scene, Train *train);
    ~WeaponWagon();

    void primaryAction() override;
    void primaryActionDebug() override;

    void setChargeProjectile(bool charge);

    void setAimVector(const QVector3D& aimVector);

    void fire(QVector3D force);

    bool isReloading() const;

    void update(int elapsedMilliseconds) override;
    void preRender(QOpenGLFunctions& gl, Program & program) const override;
    float length() const;
    unsigned int chargeTime() const;
    unsigned int reloadTime() const;

protected:
    unsigned int maxChargeMilliseconds() const;
    QVector3D getAimDeviation() const;
protected:
    bool m_chargeProjectile;
    unsigned int m_chargeTime;
    bool m_reloadProjectile;
    unsigned int m_reloadTime;
    QVector3D m_normalizedAimVector;
};

}
