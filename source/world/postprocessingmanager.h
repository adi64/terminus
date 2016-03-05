#pragma once

#include <memory>

#include <world/drawables/abstractgraphicsobject.h>

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
    void allocateFBO(QOpenGLFunctions & gl) const;
    void deallocateFBO(QOpenGLFunctions & gl) const;

    mutable bool m_objectsOnGPU;
    mutable GLuint m_fbo;
    mutable GLuint m_fboTexture;
    mutable GLuint m_rboDepth;
};

}
