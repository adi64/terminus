#include "postprocessingmanager.h"

#include <QImage>

#include <resources/resourcemanager.h>
#include <resources/program.h>
#include <world/world.h>
#include <player/localplayer.h>

namespace terminus
{

PostprocessingManager::PostprocessingManager(World &world)
: AbstractGraphicsObject(world)
, m_motionBlurEnabled(true)
, m_motionBlurFactor(3.0)
, m_frameBufferObject(world.viewport())
{
    m_program = ResourceManager::getInstance()->getProgram("postprocessing");
    m_geometry = ResourceManager::getInstance()->getGeometry("base_squad");
}

void PostprocessingManager::setMotionBlur(bool enabled)
{
    m_motionBlurEnabled = enabled;
}

bool PostprocessingManager::motionBlurEnabled() const
{
    return m_motionBlurEnabled;
}

void PostprocessingManager::setMotionBlurFactor(float factor)
{
    m_motionBlurFactor = factor;
}

float PostprocessingManager::motionBlurFactor() const
{
    return m_motionBlurFactor;
}

void PostprocessingManager::localRenderSetup(QOpenGLFunctions & gl, Program & /*program*/) const
{
    m_frameBufferObject.bindTexture(gl, GL_TEXTURE0);
}
void PostprocessingManager::localRenderCleanup(QOpenGLFunctions & gl, Program & /*program*/) const
{
    m_frameBufferObject.releaseTexture(gl, GL_TEXTURE0);

    if(m_motionBlurEnabled)
        applyMotionBlur(gl);
}

void PostprocessingManager::beforeRenderHook(QOpenGLFunctions & gl) const
{
    m_frameBufferObject.bindFBO(gl);
}

void PostprocessingManager::afterRenderHook(QOpenGLFunctions & gl) const
{
    m_frameBufferObject.releaseFBO(gl);

    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void PostprocessingManager::applyMotionBlur(QOpenGLFunctions & /*gl*/) const
{
    static bool firstFrame = true;
    if(firstFrame)
    {
        glAccum(GL_LOAD, 1.0);
        firstFrame = false;
        return;
    }

    glAccum(GL_MULT, 1.0 - (1.0 / m_motionBlurFactor));
    glAccum(GL_ACCUM, 1.0 / m_motionBlurFactor);
    glAccum(GL_RETURN, 1.0);
}

} //namespace terminus
