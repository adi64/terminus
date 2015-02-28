#pragma once

#include "abstractwagon.h"

#include <resources/lightmanager.h>

namespace terminus
{

class EngineWagon : public AbstractWagon
{


public:
    EngineWagon(World & world, Train * train);
    virtual ~EngineWagon();

    virtual void localUpdate() override;

    float length() const;

    void playSound() const;

protected:
    LightManager::ID m_headLight;
};

}
