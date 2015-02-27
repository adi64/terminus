#pragma once

#include <memory>

#include "abstractplayer.h"

namespace terminus
{

class Train;

class LocalPlayer : public AbstractPlayer
{
public:
    LocalPlayer(World & world, std::shared_ptr<Train> train);

protected:

};
}
