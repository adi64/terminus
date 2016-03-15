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

    void setMotionBlur(bool enabled = true);
    bool motionBlurEnabled() const;
    void setMotionBlurFactor(float factor);
    float motionBlurFactor() const;

    void localRenderSetup(QOpenGLFunctions & gl, Program & program) const override;
    void localRenderCleanup(QOpenGLFunctions & gl, Program & program) const override;

    void beforeRenderHook(QOpenGLFunctions &gl) const;
    void afterRenderHook(QOpenGLFunctions &gl) const;
protected:

    void applyMotionBlur(QOpenGLFunctions & gl) const;

    bool m_motionBlurEnabled;
    float m_motionBlurFactor;

    FrameBufferObject m_frameBufferObject;
};

}
