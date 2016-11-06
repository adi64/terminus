#include "compose.h"

namespace terminus
{

Compose::Compose(const World & world, DisabledBehaviour behaviour)
: AbstractEffect(world, behaviour)
{
    m_program = ResourceManager::getInstance()->getProgram("pp_compose");
}

Compose::~Compose()
{

}

void Compose::localRenderSetup(const Camera & /*camera*/, Program & program) const
{
    program.setUniform("u_Normal_Depth", 0);
    program.setUniform("u_CamCoord_Id", 1);
    program.setUniform("u_Emit", 2);
    program.setUniform("u_Diffuse_Alpha", 3);
    program.setUniform("u_Specular_Specularity", 4);
}

} //namespace terminus
