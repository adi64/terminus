#include "vignette.h"

namespace terminus
{

Vignette::Vignette(Game &world, DisabledBehaviour behaviour)
: AbstractEffect(world, behaviour)
{
    m_program = ResourceManager::getInstance()->getProgram("postprocessing_vignette");
}

Vignette::~Vignette()
{

}

} //namespace terminus
