#include "invert.h"

namespace terminus
{

Invert::Invert(World &world, DisabledBehaviour behaviour)
: AbstractEffect(world, behaviour)
{
    m_program = ResourceManager::getInstance()->getProgram("postprocessing_invert");
}

Invert::~Invert()
{

}

} //namespace terminus