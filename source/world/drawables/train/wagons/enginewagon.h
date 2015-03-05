#pragma once

#include <resources/lightmanager.h>

#include "abstractwagon.h"

namespace terminus
{

class EngineWagon : public AbstractWagon
{


public:
    EngineWagon(World & world, Train * train);
    virtual ~EngineWagon();

    virtual void localUpdate() override;

    virtual void primaryAction() override;
    virtual float cooldownRate() const override;
    virtual float maxHealth() const;

    virtual float length() const override;
    virtual WagonType wagonType() const override;

    void playSound() const;

protected:
    LightManager::ID m_headLight;
};

}
