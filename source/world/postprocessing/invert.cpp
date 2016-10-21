#include "invert.h"

namespace terminus
{

Invert::Invert(DisabledBehaviour behaviour)
: AbstractEffect(behaviour)
{
    m_program = ResourceManager::getInstance()->getProgram("postprocessing_invert");
}

Invert::~Invert()
{

}

} //namespace terminus
