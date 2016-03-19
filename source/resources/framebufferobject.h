#pragma once

#include <initializer_list>

#include <QOpenGLFunctions>
#include <QSize>

namespace terminus
{

class FrameBufferObject
{
    static constexpr unsigned int MaxColorAttachmentCount = 8;

public:
    FrameBufferObject(QSize size, std::initializer_list<int> colorAttachmentFormats = { GL_RGBA }, bool hasDepth = true);
    FrameBufferObject(int width, int height, std::initializer_list<int> colorAttachmentFormats = { GL_RGBA }, bool hasDepth = true);
    virtual ~FrameBufferObject();

    void bindFBO(QOpenGLFunctions & gl) const;
    void releaseFBO(QOpenGLFunctions & gl) const;

    void bindTexture(QOpenGLFunctions & gl, int colorAttachment = GL_COLOR_ATTACHMENT0, int textureUnit = GL_TEXTURE0) const;
    void releaseTexture(QOpenGLFunctions & gl, int textureUnit = GL_TEXTURE0) const;

protected:
    void allocateFBO(QOpenGLFunctions & gl) const;
    void deallocateFBO(QOpenGLFunctions & gl) const;

    int m_width;
    int m_height;
    int m_colorAttachmentFormats[MaxColorAttachmentCount];
    bool m_hasDepth;

    mutable bool m_objectsOnGPU;
    mutable GLuint m_fbo;
    mutable GLuint m_fboTexture[MaxColorAttachmentCount];
    mutable GLuint m_rboDepth;
};

} // namespace terminus
