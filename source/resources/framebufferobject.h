#pragma once

#include <initializer_list>
#include <memory>

#include <glincl.h>

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

    // "the screen"
    static const FrameBufferObject & defaultFBO();

    void bindFBO() const;
    void releaseFBO() const;

    void bindTexture(int colorAttachment = GL_COLOR_ATTACHMENT0, int textureUnit = GL_TEXTURE0) const;
    void releaseTexture(int textureUnit = GL_TEXTURE0) const;

    unsigned int colorAttachmentCount() const;
    bool hasColorAttachment(GLenum colorAttachmentNumber) const;

protected:
    void allocateFBO() const;
    void deallocateFBO() const;

    static std::unique_ptr<FrameBufferObject> s_defaultFBO;

    FrameBufferObject();

    int m_width;
    int m_height;
    int m_colorAttachmentFormats[MaxColorAttachmentCount];
    unsigned int m_colorAttachmentCount;
    bool m_hasDepth;

    mutable bool m_objectsOnGPU;
    mutable GLuint m_fbo;
    mutable GLuint m_fboTexture[MaxColorAttachmentCount];
    mutable GLuint m_rboDepth;
};

} // namespace terminus
