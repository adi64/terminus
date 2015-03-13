#pragma once

namespace terminus
{

/*!
 * \brief The Commands enum stores all command types with corresponding ID.
 */
enum Commands {
    Command_Pause           = 0,
    Command_PrepareNewGame  = 1,
    Command_ProjectileFired = 2,
    Command_ProjectileHit   = 3,
    Command_ClientReady     = 4,
    Command_PrimaryAction   = 5,
    Command_Sync            = 6,
    Command_GameEnded       = 7
};

} // namespace terminus
