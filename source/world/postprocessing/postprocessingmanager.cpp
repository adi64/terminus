#include "postprocessingmanager.h"

#include <QImage>

#include <resources/resourcemanager.h>
#include <resources/program.h>
#include <world/world.h>
#include <player/localplayer.h>

#include <world/postprocessing/abstracteffect.h>
#include <world/postprocessing/motionblur.h>
namespace terminus
{

PostprocessingManager::PostprocessingManager(World &world)
: m_world(world)
, m_frameBufferObject(world.viewport())
, m_motionBlur(std::unique_ptr<MotionBlur>(new MotionBlur(world)))
{
}

void PostprocessingManager::beforeRenderHook(QOpenGLFunctions & gl) const
{
    m_frameBufferObject.bindFBO(gl);
}

void PostprocessingManager::afterRenderHook(QOpenGLFunctions & gl) const
{
    m_frameBufferObject.releaseFBO(gl);

    // clear real framebuffer
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void PostprocessingManager::applyEffects(QOpenGLFunctions & gl)
{
    applyEffect(gl, *m_motionBlur);
}

void PostprocessingManager::applyEffect(QOpenGLFunctions & gl, AbstractEffect &effect)
{
    //m_frameBufferObject.bindFBO(gl);

    m_frameBufferObject.bindTexture(gl);

    effect.render(gl);

    m_frameBufferObject.releaseTexture(gl);

    //m_frameBufferObject.releaseFBO(gl);
}

} //namespace terminus
