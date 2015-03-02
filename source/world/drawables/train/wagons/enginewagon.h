#pragma once

#include "abstractwagon.h"

namespace terminus
{

class EngineWagon : public AbstractWagon
{


public:
    EngineWagon(World & world, Train * train);
    virtual ~EngineWagon();

    virtual void primaryAction() override;
    float cooldownRate() const override;
    virtual void localUpdate() override;

    float length() const;
    WagonType wagonType() const override;
    void playSound() const;
};

}
