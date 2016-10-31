#pragma once

#include <memory>
#include <vector>

#include <resources/framebufferobject.h>

#include <world/postprocessing/abstracteffect.h>
#include <world/postprocessing/compose.h>
#include <world/postprocessing/passthrough.h>

namespace terminus
{

class Game;



class PostprocessingManager
{
public:
    PostprocessingManager(Game & world);

    /* Color Attachment mapping:
     * CA0: normal + depth
     * CA1: cameraspace coordinates + object id
     * CA2: emissive color + 0
     * CA3: diffuse color + alpha
     * CA4: specular color + specularity
     */
    const FrameBufferObject & gBufferFBO() const;

    void composeImage();
protected:
    void applyEffect(AbstractEffect * effect, FrameBufferObject * sourceFBO, FrameBufferObject * targetFBO);

    Game & m_world;
    FrameBufferObject m_frameBufferObject;

    std::unique_ptr<Compose> m_compose;
    std::unique_ptr<Passthrough> m_passthrough;

    std::vector<std::unique_ptr<AbstractEffect>> m_effects;
    std::vector<std::unique_ptr<FrameBufferObject>> m_effectFBOs;
};

}
