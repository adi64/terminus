#include "framebufferobject.h"

#include <QDebug>

namespace terminus
{

FrameBufferObject::FrameBufferObject(QSize size)
: FrameBufferObject(size.width(), size.height())
{
}

FrameBufferObject::FrameBufferObject(int width, int height)
: m_width(width)
, m_height(height)
, m_objectsOnGPU(false)
{

}

FrameBufferObject::~FrameBufferObject()
{

}

void FrameBufferObject::bindFBO(QOpenGLFunctions &gl) const
{
    if(!m_objectsOnGPU)
        allocateFBO(gl);

    // switch render target
    gl.glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void FrameBufferObject::releaseFBO(QOpenGLFunctions &gl) const
{
    // switch back to "real" framebuffer
    gl.glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // clear framebuffer
    //glClearColor(1.0, 0.0, 0.0, 1.0);
    //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void FrameBufferObject::bindTexture(QOpenGLFunctions & gl, int textureUnit) const
{
    gl.glActiveTexture(textureUnit);
    gl.glEnable(GL_TEXTURE_2D);
    gl.glBindTexture(GL_TEXTURE_2D, m_fboTexture);
}

void FrameBufferObject::releaseTexture(QOpenGLFunctions & gl, int textureUnit) const
{
    gl.glActiveTexture(textureUnit);
    gl.glBindTexture(GL_TEXTURE_2D, 0);
}

void FrameBufferObject::allocateFBO(QOpenGLFunctions &gl) const
{
    if(m_objectsOnGPU)
        return;

    qDebug() << m_width << " " << m_height;

    m_fbo = -1;
    m_fboTexture = -1;
    m_rboDepth = -1;

    // Texture
    gl.glActiveTexture(GL_TEXTURE0);
    gl.glGenTextures(1, &m_fboTexture);
    gl.glBindTexture(GL_TEXTURE_2D, m_fboTexture);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    gl.glBindTexture(GL_TEXTURE_2D, 0);

    // Depth buffer
    gl.glGenRenderbuffers(1, &m_rboDepth);
    gl.glBindRenderbuffer(GL_RENDERBUFFER, m_rboDepth);
    gl.glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_width, m_height);
    gl.glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // Framebuffer to link everything together
    gl.glGenFramebuffers(1, &m_fbo);
    gl.glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    gl.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fboTexture, 0);
    gl.glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rboDepth);

    GLenum status;
    if ((status = gl.glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
    {
        qDebug() << "glCheckFramebufferStatus: error " << status;
    }

    gl.glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_objectsOnGPU = true;
}

void FrameBufferObject::deallocateFBO(QOpenGLFunctions & gl) const
{
    if(!m_objectsOnGPU)
        return;

    gl.glDeleteRenderbuffers(1, &m_rboDepth);
    gl.glDeleteTextures(1, &m_fboTexture);
    gl.glDeleteFramebuffers(1, &m_fbo);

    m_objectsOnGPU = false;
}

} // namespace terminus
