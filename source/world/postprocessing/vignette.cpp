#include "vignette.h"

namespace terminus
{

Vignette::Vignette(DisabledBehaviour behaviour)
: AbstractEffect(behaviour)
{
    m_program = ResourceManager::getInstance()->getProgram("postprocessing_vignette");
}

Vignette::~Vignette()
{

}

} //namespace terminus
