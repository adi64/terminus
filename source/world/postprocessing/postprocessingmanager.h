#pragma once

#include <memory>

#include <resources/framebufferobject.h>

#include <world/postprocessing/abstracteffect.h>
#include <world/postprocessing/motionblur.h>

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
    void applyEffect(QOpenGLFunctions &gl, AbstractEffect & effect);

    World & m_world;
    FrameBufferObject m_frameBufferObject;

    std::unique_ptr<MotionBlur> m_motionBlur;
};

}
