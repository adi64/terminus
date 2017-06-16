#include "framebufferobject.h"

#include <QDebug>

namespace terminus
{

std::unique_ptr<FrameBufferObject> FrameBufferObject::s_defaultFBO;

FrameBufferObject::FrameBufferObject(QSize size, std::initializer_list<int> colorAttachmentFormats, bool hasDepth)
: FrameBufferObject(size.width(), size.height(), colorAttachmentFormats, hasDepth)
{
}

FrameBufferObject::FrameBufferObject(int width, int height, std::initializer_list<int> colorAttachmentFormats, bool hasDepth)
: m_width(width)
, m_height(height)
, m_colorAttachmentCount(colorAttachmentFormats.size())
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

const FrameBufferObject &FrameBufferObject::defaultFBO()
{
    // Get default FBO (i.e. "the screen")
    // Lazy initialize, because this needs a valid OpenGL context that we don't have at static initialization time
    if (!s_defaultFBO)
        s_defaultFBO = std::unique_ptr<FrameBufferObject>(new FrameBufferObject());

    return *s_defaultFBO;
}

void FrameBufferObject::bindFBO() const
{
    if(!m_objectsOnGPU)
        allocateFBO();

    gl.glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void FrameBufferObject::releaseFBO() const
{
    gl.glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferObject::bindTexture(int colorAttachment, int textureUnit) const
{
    unsigned int textureNumber = static_cast<unsigned int>(colorAttachment - GL_COLOR_ATTACHMENT0);
    if(textureNumber >= MaxColorAttachmentCount)
        return;

    gl.glActiveTexture(textureUnit);
    gl.glEnable(GL_TEXTURE_2D);
    gl.glBindTexture(GL_TEXTURE_2D, m_fboTexture[textureNumber]);
}

void FrameBufferObject::releaseTexture(int textureUnit) const
{
    gl.glActiveTexture(textureUnit);
    gl.glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int FrameBufferObject::colorAttachmentCount() const
{
    return m_colorAttachmentCount;
}

bool FrameBufferObject::hasColorAttachment(GLenum colorAttachmentNumber) const
{
    auto offset = colorAttachmentNumber - GL_COLOR_ATTACHMENT0;
    return colorAttachmentCount() >= offset;
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
            //TODO do we need this?! gl.glActiveTexture(GL_TEXTURE0);
            gl.glGenTextures(1, &(m_fboTexture[i]));
            gl.glBindTexture(GL_TEXTURE_2D, m_fboTexture[i]);
            gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            gl.glTexImage2D(GL_TEXTURE_2D, 0, m_colorAttachmentFormats[i], m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            gl.glBindTexture(GL_TEXTURE_2D, 0);
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
        gl.glGenRenderbuffers(1, &m_rboDepth);
        gl.glBindRenderbuffer(GL_RENDERBUFFER, m_rboDepth);
        gl.glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_width, m_height);
        gl.glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }
    else
    {
        m_rboDepth = 0;
    }

    // Framebuffer to link everything together
    gl.glGenFramebuffers(1, &m_fbo);
    gl.glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    for(unsigned int i = 0; i < MaxColorAttachmentCount; ++i)
    {
        gl.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_fboTexture[i], 0);
    }
    if(m_hasDepth)
    {
        gl.glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rboDepth);
    }

    gl.glDrawBuffers(drawBufferCount, drawBuffers);

    GLenum status;
    if ((status = gl.glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
    {
        qDebug() << "glCheckFramebufferStatus: error " << status;
    }

    gl.glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_objectsOnGPU = true;
}

void FrameBufferObject::deallocateFBO() const
{
    if(!m_objectsOnGPU)
        return;

    gl.glDeleteRenderbuffers(1, &m_rboDepth);
    gl.glDeleteTextures(8, m_fboTexture);
    gl.glDeleteFramebuffers(1, &m_fbo);

    m_objectsOnGPU = false;
}

// "the screen"
FrameBufferObject::FrameBufferObject()
: m_width(-1)
, m_height(-1)
, m_colorAttachmentCount(1)
, m_hasDepth(false)
, m_objectsOnGPU(true)
{
    // get ID of default framebuffer
    GLint defaultFBO;
    gl.glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFBO);
    qDebug() << "Default framebuffer ID: " << defaultFBO;

    m_fbo = defaultFBO;
}

} // namespace terminus
