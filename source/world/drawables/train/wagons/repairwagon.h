#pragma once

#include "abstractwagon.h"

namespace terminus
{

class RepairWagon : public AbstractWagon
{
public:
    RepairWagon(World & world, Train * train);
    virtual ~RepairWagon();

    virtual void localUpdate() override;

    virtual void primaryAction() override;
    virtual float cooldownTime() const override;

    virtual WagonType wagonType() const override;
};

}
