#pragma once

#include <memory>

#include "abstractplayer.h"

namespace terminus
{

class Train;

class LocalPlayer : public AbstractPlayer
{
public:
    LocalPlayer(World & world, Train *train);

    virtual void primaryAction() override;
    virtual void primaryActionDebug() override;

protected:

};
}
