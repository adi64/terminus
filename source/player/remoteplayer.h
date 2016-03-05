#pragma once

#include <memory>

#include "abstractplayer.h"

namespace terminus
{

class Train;

/*!
 * \brief The RemotePlayer class represents a player whose actions get
 * transmitted over network. It does not contain any logic; the sole purpose
 * of this class is to distinguish different player types.
 */
class RemotePlayer : public AbstractPlayer
{
public:
    RemotePlayer(World & world, Train *train);

protected:

};
}
