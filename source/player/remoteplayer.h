#pragma once

#include <memory>

#include "abstractplayer.h"

namespace terminus
{

class Train;

class RemotePlayer : public AbstractPlayer
{
public:
    RemotePlayer(Train *train);

protected:

};
}
