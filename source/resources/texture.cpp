#include "texture.h"

#include <GLES3/gl3.h>

namespace terminus
{

Texture::Texture(GLenum type, GLenum internalFormat)
: m_type(type)
, m_format(internalFormat)
, m_width(1)
, m_height(1)
, m_dimensionsChanged(true)
, m_minFilter(GL_NEAREST)
, m_magFilter(GL_NEAREST)
, m_wrap(GL_CLAMP_TO_EDGE)
, m_parametersChanged(true)
, m_hasData(false)
, m_isOnGPU(false)
{
}

Texture::~Texture()
{
    if (m_isOnGPU)
    {
        deallocate();
    }
}

void Texture::setData(GLuint width, GLuint height, GLenum format, GLenum type, void * data, GLuint dataLength)
{
    m_width = width;
    m_height = height;
    m_dimensionsChanged = true;

    m_dataFormat = format;
    m_dataType = type;
    m_data = std::vector<unsigned char>(static_cast<unsigned char *>(data),
                                        static_cast<unsigned char *>(data) + dataLength);
    m_hasData = true;
}

void Texture::resize(GLuint width, GLuint height)
{
    m_width = width;
    m_height = height;
    m_dimensionsChanged = true;

    m_hasData = false;
}

void Texture::setFilter(GLenum filter)
{
    setFilter(filter, filter);
}

void Texture::setFilter(GLenum minFilter, GLenum magFilter)
{
    m_minFilter = minFilter;
    m_magFilter = magFilter;
    m_parametersChanged = true;
}

void Texture::setWrap(GLenum wrap)
{
    m_wrap = wrap;
    m_parametersChanged = true;
}

void Texture::bind() const
{
    synchronizeState();
    glBindTexture(m_type, m_objectID);
}

void Texture::release() const
{
    glBindTexture(m_type, 0);
}

void Texture::bindTo(GLuint textureUnit) const
{
    synchronizeState();
    glActiveTexture(textureUnit);

}

void Texture::releaseFrom(GLuint textureUnit) const
{
    glActiveTexture(textureUnit);

}

void Texture::synchronizeState() const
{
    if (m_dimensionsChanged)
        deallocate();

    if (!m_isOnGPU)
    {
        glGenTextures(1, &m_objectID);
        glBindTexture(m_type, m_objectID);
        // TODO-LW use appropriate TexImage/TexStorage call depending on m_type
        if (m_hasData)
        {
            glTexStorage2D(m_type, 0, m_format, m_width, m_height);
        }
        else
        {
            glTexImage2D(m_type, 0, m_format, m_width, m_height, 0,
                            m_dataFormat, m_dataType, m_data.data());
        }
        glBindTexture(m_type, 0);
        m_dimensionsChanged = false;
    }

    if (m_parametersChanged)
    {
        glBindTexture(m_type, m_objectID);
        glTexParameteri(m_type, GL_TEXTURE_MAG_FILTER, m_magFilter);
        glTexParameteri(m_type, GL_TEXTURE_MIN_FILTER, m_magFilter);
        glTexParameteri(m_type, GL_TEXTURE_WRAP_S, m_wrap);
        glTexParameteri(m_type, GL_TEXTURE_WRAP_T, m_wrap);
        glTexParameteri(m_type, GL_TEXTURE_WRAP_R, m_wrap);
        glBindTexture(m_type, 0);
        m_parametersChanged = false;
    }
}

void Texture::deallocate() const
{
    glDeleteTextures(1, &m_objectID);
    m_isOnGPU = false;
}

}; // namespace terminus

