#include "motionblur.h"

#include <GLES3/gl3.h>


namespace terminus
{

MotionBlur::MotionBlur(Game &world, DisabledBehaviour behaviour)
: AbstractEffect(world, behaviour)
, m_motionBlurFactor(3.0)
{
    m_program = ResourceManager::getInstance()->getProgram("postprocessing_passthrough");
}

MotionBlur::~MotionBlur()
{

}

void MotionBlur::localRenderSetup(Program & /*program*/) const
{

}

void MotionBlur::localRenderCleanup(Program & /*program*/) const
{
    //TODO: replace with shader code. glAccum is not supported in GLES
//    static bool firstFrame = true;
//    if(firstFrame)
//    {
//        glAccum(GL_LOAD, 1.0);
//        firstFrame = false;
//        return;
//    }

//    glAccum(GL_MULT, 1.0 - (1.0 / m_motionBlurFactor));
//    glAccum(GL_ACCUM, 1.0 / m_motionBlurFactor);
//    glAccum(GL_RETURN, 1.0);
}

} //namespace terminus
