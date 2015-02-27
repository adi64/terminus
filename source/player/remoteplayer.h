#pragma once

#include <memory>

#include "abstractplayer.h"

namespace terminus
{

class Train;

class RemotePlayer : public AbstractPlayer
{
public:
    RemotePlayer(World & world, Train *train);

protected:

};
}
