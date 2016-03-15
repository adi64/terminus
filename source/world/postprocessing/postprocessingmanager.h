#pragma once

#include <memory>

#include <world/drawables/abstractgraphicsobject.h>
#include <resources/framebufferobject.h>

class QImage;
class QOpenGLFunctions;

namespace terminus
{

class PostprocessingManager : public AbstractGraphicsObject
{
public:
    PostprocessingManager(World & world);

    void localRenderSetup(QOpenGLFunctions & gl, Program & program) const override;
    void localRenderCleanup(QOpenGLFunctions & gl, Program & program) const override;

    void beforeRenderHook(QOpenGLFunctions &gl) const;
    void afterRenderHook(QOpenGLFunctions &gl) const;
protected:

    FrameBufferObject m_frameBufferObject;
};

}
