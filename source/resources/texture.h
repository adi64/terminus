#pragma once

#include <vector>

#include <GLES3/gl3.h>


namespace terminus
{

class Texture
{
public:
    Texture(GLenum type, GLenum internalFormat);
    ~Texture();
    
    void setData(GLuint width, GLuint height, GLenum format, GLenum type, void * data, GLuint dataLength);
    void resize(GLuint width, GLuint height);

    void setFilter(GLenum filter);
    void setFilter(GLenum minFilter, GLenum magFilter);
    void setWrap(GLenum mag);

    void bind() const;
    void release() const;

    void bindTo(GLuint textureUnit) const;
    void releaseFrom(GLuint textureUnit) const;

protected:
    void synchronizeState() const;
    void deallocate() const;

protected:
    GLenum m_type; // TODO-LW rename to m_target
    GLenum m_format;
    GLuint m_width;
    GLuint m_height;
    GLuint m_depth;
    mutable bool m_dimensionsChanged;
    
    GLenum m_minFilter;
    GLenum m_magFilter;
    GLenum m_wrap;
    mutable bool m_parametersChanged;

    GLenum m_dataFormat;
    GLenum m_dataType;
    std::vector<unsigned char> m_data;
    bool m_hasData;

    mutable GLuint m_objectID;
    mutable GLenum m_isOnGPU;
};

}; // namespace terminus

