#include "compose.h"

namespace terminus
{

Compose::Compose(DisabledBehaviour behaviour)
: AbstractEffect(behaviour)
{
    m_program = ResourceManager::getInstance()->getProgram("pp_compose");
}

Compose::~Compose()
{

}

void Compose::localRenderSetup(Program & program) const
{
    program.setUniform("u_Normal_Depth", 0);
    program.setUniform("u_CamCoord_Id", 1);
    program.setUniform("u_Emit", 2);
    program.setUniform("u_Diffuse_Alpha", 3);
    program.setUniform("u_Specular_Specularity", 4);
}

} //namespace terminus
