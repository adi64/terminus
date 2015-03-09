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
