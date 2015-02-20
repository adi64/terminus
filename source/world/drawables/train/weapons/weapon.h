#pragma once

#include <world/physics/kinematicphysicsobject.h>

namespace terminus
{

class Turret;
class Barrel;

class Weapon : public AbstractGraphicsObject
{

public:
    Weapon(std::shared_ptr<Scene> scene);
    virtual ~Weapon();

public:
    virtual void render(QOpenGLFunctions & gl) const override;

    void fire();
    virtual void update(int elapsedMilliseconds, QVector3D position, QQuaternion rotation);

    QVector3D weaponOffset();

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
    float m_damage;
    float m_reloadTime;
    float m_scattering;
    float m_thrust;
    int m_magazineSize;

protected:
    std::unique_ptr<Turret> m_turret;
    std::unique_ptr<Barrel> m_barrel;

};

} //namespace
