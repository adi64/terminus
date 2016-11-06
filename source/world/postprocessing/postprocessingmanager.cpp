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
, m_frameBufferObject(world.viewport(), {GL_RGBA16F, GL_RGBA16F, GL_RGBA16F, GL_RGBA16F, GL_RGBA16F})
{
    // initialize effects
    m_compose = std::unique_ptr<Compose>(new Compose(world));
    m_compose->setTextures(
        {
            {m_frameBufferObject, GL_COLOR_ATTACHMENT0},
            {m_frameBufferObject, GL_COLOR_ATTACHMENT1},
            {m_frameBufferObject, GL_COLOR_ATTACHMENT2},
            {m_frameBufferObject, GL_COLOR_ATTACHMENT3},
            {m_frameBufferObject, GL_COLOR_ATTACHMENT4}
        });


    auto motionBlurPtr = std::unique_ptr<MotionBlur>(new MotionBlur(world));
    motionBlurPtr->setTextures(
        {
            {m_compose->targetFBO(), GL_COLOR_ATTACHMENT0},
            {m_frameBufferObject, GL_COLOR_ATTACHMENT0} // normal xyz + depth
        });
//    auto vignettePtr = std::unique_ptr<Vignette>(new Vignette(world));
//    vignettePtr->setTextures(
//        {
//            {motionBlurPtr->targetFBO(), GL_COLOR_ATTACHMENT0}
//        });

    // create list of effects
    m_effects.push_back(std::move(motionBlurPtr));
//    m_effects.push_back(std::move(vignettePtr));

    // last "effect" to render to screen instead of texture
    m_passthrough = std::unique_ptr<Passthrough>(new Passthrough(world));
    m_passthrough->setTextures(
        {
            {m_effects.back()->targetFBO(), GL_COLOR_ATTACHMENT0}
        });
}

const FrameBufferObject & PostprocessingManager::gBufferFBO() const
{
    return m_frameBufferObject;
}

void PostprocessingManager::composeImage()
{
    // Compose image from GBuffer
    m_compose->render(m_world.localPlayer().camera(), m_world.lightManager());

    // Apply post-processing effects
    for (const auto & effectPtr : m_effects)
    {
        effectPtr->render(m_world.localPlayer().camera(), m_world.lightManager());
    }

    // Render last image to screen
    m_passthrough->render(m_world.localPlayer().camera(), m_world.lightManager());
}

} //namespace terminus
