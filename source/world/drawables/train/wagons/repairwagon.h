#pragma once

#include "abstractwagon.h"

namespace terminus
{

/*!
 * \brief The RepairWagon class provides a wagon type, which primary action increases the health of all still functional weagons by a fix percentage.
 */
class RepairWagon : public AbstractWagon
{
public:
    RepairWagon(World & world, Train * train);
    virtual ~RepairWagon();

    virtual void localUpdate() override;

    virtual void primaryActionInternal() override;
    virtual float cooldownTime() const override;

    virtual WagonType wagonType() const override;
};

}
