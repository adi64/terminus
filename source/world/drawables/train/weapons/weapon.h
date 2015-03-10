#pragma once

#include <world/physics/kinematicphysicsobject.h>

namespace terminus
{

class Turret;
class Barrel;

class Weapon : public AbstractGraphicsObject
{

public:
    Weapon(World & world);
    virtual ~Weapon();

public:
    void fire();
    virtual void localUpdate() override;

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
    virtual void doForAllChildren(std::function<void(AbstractGraphicsObject &)> callback) override;
    virtual bool localRenderEnabled() const override;

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
