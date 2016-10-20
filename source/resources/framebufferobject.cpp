#include "framebufferobject.h"

#include <QDebug>

namespace terminus
{

FrameBufferObject::FrameBufferObject(QSize size, std::initializer_list<int> colorAttachmentFormats, bool hasDepth)
: FrameBufferObject(size.width(), size.height(), colorAttachmentFormats, hasDepth)
{
}

FrameBufferObject::FrameBufferObject(int width, int height, std::initializer_list<int> colorAttachmentFormats, bool hasDepth)
: m_width(width)
, m_height(height)
, m_hasDepth(hasDepth)
, m_objectsOnGPU(false)
{
    for(unsigned int i = 0; i < MaxColorAttachmentCount; ++i)
    {
        if(i < colorAttachmentFormats.size())
        {
            m_colorAttachmentFormats[i] = colorAttachmentFormats.begin()[i];
        }
        else
        {
            m_colorAttachmentFormats[i] = GL_NONE;
        }
    }

}

FrameBufferObject::~FrameBufferObject()
{
    // TODO FIXME we should call deallocateFBO() here,
    // but where do we get a valid OpenGL context?
    // On program teardown this doesn't matter...
}

void FrameBufferObject::bindFBO() const
{
    if(!m_objectsOnGPU)
        allocateFBO();

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void FrameBufferObject::releaseFBO() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferObject::bindTexture(int colorAttachment, int textureUnit) const
{
    unsigned int textureNumber = static_cast<unsigned int>(colorAttachment - GL_COLOR_ATTACHMENT0);
    if(textureNumber >= MaxColorAttachmentCount)
        return;

    glActiveTexture(textureUnit);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_fboTexture[textureNumber]);
}

void FrameBufferObject::releaseTexture(int textureUnit) const
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void FrameBufferObject::allocateFBO() const
{
    if(m_objectsOnGPU)
        return;

    qDebug() << m_width << " " << m_height;

    m_fbo = -1;

    GLenum drawBuffers[MaxColorAttachmentCount];
    GLsizei drawBufferCount = 0;

    // Texture
    for(unsigned int i = 0; i < MaxColorAttachmentCount; ++i)
    {
        if (m_colorAttachmentFormats[i] != GL_NONE)
        {
            //TODO do we need this?! glActiveTexture(GL_TEXTURE0);
            glGenTextures(1, &(m_fboTexture[i]));
            glBindTexture(GL_TEXTURE_2D, m_fboTexture[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, m_colorAttachmentFormats[i], m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            glBindTexture(GL_TEXTURE_2D, 0);
            drawBuffers[drawBufferCount] = GL_COLOR_ATTACHMENT0 + drawBufferCount;
            drawBufferCount++;
        }
        else
        {
            m_fboTexture[i] = 0;
        }
    }

    // Depth buffer
    if(m_hasDepth)
    {
        glGenRenderbuffers(1, &m_rboDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, m_rboDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_width, m_height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }
    else
    {
        m_rboDepth = 0;
    }

    // Framebuffer to link everything together
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    for(unsigned int i = 0; i < MaxColorAttachmentCount; ++i)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_fboTexture[i], 0);
    }
    if(m_hasDepth)
    {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rboDepth);
    }

    glDrawBuffers(drawBufferCount, drawBuffers);

    GLenum status;
    if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
    {
        qDebug() << "glCheckFramebufferStatus: error " << status;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_objectsOnGPU = true;
}

void FrameBufferObject::deallocateFBO() const
{
    if(!m_objectsOnGPU)
        return;

    glDeleteRenderbuffers(1, &m_rboDepth);
    glDeleteTextures(8, m_fboTexture);
    glDeleteFramebuffers(1, &m_fbo);

    m_objectsOnGPU = false;
}

} // namespace terminus
