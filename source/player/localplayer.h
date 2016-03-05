#pragma once

#include <memory>

#include "abstractplayer.h"

namespace terminus
{

class Train;

/*!
 * \brief The LocalPlayer class represents a local player that interacts with
 * the World and their Train. It does not contain any logic; the sole purpose
 * of this class is to distinguish different player types.
 */
class LocalPlayer : public AbstractPlayer
{
public:
    LocalPlayer(World & world, Train *train);
};
}
