#pragma once

#include <string>

#include <QMatrix4x4>
#include <QMatrix3x3>
#include <QVector4D>
#include <QVector3D>

class QOpenGLShaderProgram;

namespace terminus
{

/*!
 * \brief The program class implements the basic functionality used for binding programs to openGL
 */
class Program
{
public:
    Program();
    Program(std::string vertexSrc, std::string fragmentSrc);
    virtual ~Program();

    /*!
     * \brief Allocates space on the GPU
     *
     * Space is allocated only, when current data isn't stored on GPU yet.
     */
    virtual void allocate();

    /*!
     * \brief Deallocates space on the GPU
     *
     * Space is deallocated only, when current data is present on GPU.
     */
    virtual void deallocate();

    virtual void bind();
    virtual void release();

    virtual QOpenGLShaderProgram & program();

    virtual void bindAttributeLocation(std::string name, int location);
    virtual void setUniform(std::string name, const QMatrix4x4 & value);
    virtual void setUniform(std::string name, const QMatrix3x3 & value);
    virtual void setUniform(std::string name, const QVector3D value);
    virtual void setUniform(std::string name, const QVector4D value);
    virtual void setUniform(std::string name, float value);
    virtual void setUniform(std::string name, int value);
    virtual void setUniform(std::string name, const QVector4D * values, int count);

private:
    std::string m_vertexSrc;
    std::string m_fragmentSrc;

    QOpenGLShaderProgram * m_program;

    bool m_isBound;
    bool m_isOnGPU;
};

} //namespace terminus
