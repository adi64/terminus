#include "invert.h"

namespace terminus
{

Invert::Invert(const World &world, DisabledBehaviour behaviour)
: AbstractEffect(world, behaviour)
{
    m_program = ResourceManager::getInstance()->getProgram("pp_invert");
}

Invert::~Invert()
{

}

} //namespace terminus
