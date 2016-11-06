#include "motionblur.h"

#include <GLES3/gl3.h>

#include <player/localplayer.h>

namespace terminus
{

MotionBlur::MotionBlur(const World &world, DisabledBehaviour behaviour)
: AbstractEffect(world, behaviour)
, m_initialized(false)
, m_motionBlurFactor(3)
{
    m_program = ResourceManager::getInstance()->getProgram("pp_motionblur");
}

MotionBlur::~MotionBlur()
{

}

void MotionBlur::localRenderSetup(const Camera & camera, Program & program) const
{
    if (!m_initialized)
    {
        m_previousViewProjectionMatrix = camera.viewProjection();
        m_initialized = true;
    }

    QMatrix4x4 viewProjectionInvertedMatrix = camera.viewProjectionInverted();

    program.setUniform("u_motionBlurFactor", m_motionBlurFactor);
    program.setUniform("u_depthTexture", 1);
    program.setUniform("u_mViewProjectionInverted", viewProjectionInvertedMatrix);
    program.setUniform("u_mPreviousViewProjection", m_previousViewProjectionMatrix);
}

void MotionBlur::localRenderCleanup(const Camera & camera, Program & /*program*/) const
{
    m_previousViewProjectionMatrix = camera.viewProjection();

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
