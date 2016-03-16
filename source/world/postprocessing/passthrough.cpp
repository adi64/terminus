#include "passthrough.h"

namespace terminus
{

Passthrough::Passthrough(World &world, DisabledBehaviour behaviour)
: AbstractEffect(world, behaviour)
{
    m_program = ResourceManager::getInstance()->getProgram("postprocessing_passthrough");
}

Passthrough::~Passthrough()
{

}

} //namespace terminus
