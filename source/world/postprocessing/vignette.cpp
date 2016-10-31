#include "vignette.h"

namespace terminus
{

Vignette::Vignette(const World &world, DisabledBehaviour behaviour)
: AbstractEffect(world, behaviour)
{
    m_program = ResourceManager::getInstance()->getProgram("pp_vignette");
}

Vignette::~Vignette()
{

}

} //namespace terminus
