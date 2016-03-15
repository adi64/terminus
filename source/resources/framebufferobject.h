#pragma once

#include <QOpenGLFunctions>
#include <QSize>

namespace terminus
{

class FrameBufferObject
{
public:
    FrameBufferObject(QSize size);
    FrameBufferObject(int width, int height);
    virtual ~FrameBufferObject();

    void bindFBO(QOpenGLFunctions &gl) const;
    void releaseFBO(QOpenGLFunctions &gl) const;

    void bindTexture(QOpenGLFunctions & gl, int textureUnit = GL_TEXTURE0) const;
    void releaseTexture(QOpenGLFunctions & gl, int textureUnit = GL_TEXTURE0) const;

protected:
    void allocateFBO(QOpenGLFunctions &gl) const;
    void deallocateFBO(QOpenGLFunctions &gl) const;

    int m_width;
    int m_height;

    mutable bool m_objectsOnGPU;
    mutable GLuint m_fbo;
    mutable GLuint m_fboTexture;
    mutable GLuint m_rboDepth;
};

} // namespace terminus
