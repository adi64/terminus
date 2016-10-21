#include "passthrough.h"

namespace terminus
{

Passthrough::Passthrough(DisabledBehaviour behaviour)
: AbstractEffect(behaviour)
{
    m_program = ResourceManager::getInstance()->getProgram("pp_passthrough");
}

Passthrough::~Passthrough()
{

}

void Passthrough::localRenderSetup(Program & program) const
{
    program.setUniform("inputTexture", 0);
}

} //namespace terminus
