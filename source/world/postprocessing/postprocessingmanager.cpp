#include "postprocessingmanager.h"

#include <QImage>

#include <resources/resourcemanager.h>
#include <resources/program.h>
#include <world/world.h>
#include <player/localplayer.h>

#include <world/postprocessing/abstracteffect.h>
#include <world/postprocessing/motionblur.h>
#include <world/postprocessing/passthrough.h>
#include <world/postprocessing/invert.h>
#include <world/postprocessing/vignette.h>

namespace terminus
{

PostprocessingManager::PostprocessingManager(World &world)
: m_world(world)
, m_frameBufferObject(world.viewport())
{
    // initialize effects
    auto motionBlurPtr = std::unique_ptr<MotionBlur>(new MotionBlur(world));
    auto invertPtr = std::unique_ptr<Invert>(new Invert(world));
    auto vignettePtr = std::unique_ptr<Vignette>(new Vignette(world));

    // create list of effects
    m_effects.push_back(std::move(motionBlurPtr));
    m_effects.push_back(std::move(invertPtr));
    m_effects.push_back(std::move(vignettePtr));

    // create FBO per effect
    for(unsigned int i=0; i<m_effects.size(); i++)
    {
        auto fboPtr = std::unique_ptr<FrameBufferObject>(new FrameBufferObject(world.viewport()));

        m_effectFBOs.push_back(std::move(fboPtr));
    }

    // last "effect" to render to screen instead of texture
    m_passthrough = std::unique_ptr<Passthrough>(new Passthrough(world));
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
    assert(m_effects.size() == m_effectFBOs.size());

    for(unsigned int i = 0; i<m_effects.size(); i++)
    {
        AbstractEffect * effectPtr = m_effects[i].get();
        FrameBufferObject * targetFBOPtr = m_effectFBOs[i].get();
        FrameBufferObject * sourceFBOPtr = &m_frameBufferObject;

        if(i > 0)
            sourceFBOPtr = m_effectFBOs[i-1].get();

        if(i == m_effects.size() -1)
            targetFBOPtr = nullptr;

        applyEffect(gl, effectPtr, sourceFBOPtr, targetFBOPtr);
    }
}

void PostprocessingManager::applyEffect(QOpenGLFunctions & gl, AbstractEffect * effect, FrameBufferObject * sourceFBO, FrameBufferObject * targetFBO)
{
    if(targetFBO)
        targetFBO->bindFBO(gl);

    sourceFBO->bindTexture(gl);

    effect->render(gl);

    sourceFBO->releaseTexture(gl);

    if(targetFBO)
        targetFBO->releaseFBO(gl);
}

} //namespace terminus
