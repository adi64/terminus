#pragma once

#include <memory>
#include <vector>

#include <resources/framebufferobject.h>

#include <world/postprocessing/abstracteffect.h>
#include <world/postprocessing/passthrough.h>

class QOpenGLFunctions;

namespace terminus
{

class World;

class PostprocessingManager
{
public:
    PostprocessingManager(World & world);

    void beforeRenderHook(QOpenGLFunctions &gl) const;
    void afterRenderHook(QOpenGLFunctions &gl) const;

    void applyEffects(QOpenGLFunctions &gl);
protected:
    void applyEffect(QOpenGLFunctions &gl, AbstractEffect * effect, FrameBufferObject * sourceFBO, FrameBufferObject * targetFBO);

    World & m_world;
    FrameBufferObject m_frameBufferObject;

    std::unique_ptr<Passthrough> m_passthrough;

    std::vector<std::unique_ptr<AbstractEffect>> m_effects;
    std::vector<std::unique_ptr<FrameBufferObject>> m_effectFBOs;
};

}
