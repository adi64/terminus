#pragma once

#include <world/drawables/train/wagons/abstractwagon.h>
#include <world/lightmanager.h>

namespace terminus
{

/*!
 * \brief The EngineWagon class provides a wagon type, which primary action increases the trains velocity.
 */
class EngineWagon : public AbstractWagon
{

public:
    EngineWagon(Game & world, Train * train);
    virtual ~EngineWagon();

    virtual void localUpdate() override;

    virtual void primaryActionInternal() override;

    virtual float maxHealth() const;

    virtual float cooldownTime() const override;

    virtual WagonType wagonType() const override;

    void playSound() const;

protected:
    virtual QVector3D lightPosition();

protected:
    LightManager::ID m_headLight;
};

}
