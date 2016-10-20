#pragma once

#include <string>

#include <QMatrix4x4>
#include <QMatrix3x3>
#include <QVector4D>
#include <QVector3D>

namespace terminus
{

/*!
 * \brief The program class implements the basic functionality used for binding programs to openGL
 */
class Program
{
public:
    Program();
    Program(std::vector<char> && vertexSrc, std::vector<char> && fragmentSrc);
    virtual ~Program();

    /*!
     * \brief Allocates space on the GPU
     *
     * Space is allocated only, when current data isn't stored on GPU yet.
     */
    virtual void allocate() const;

    /*!
     * \brief Deallocates space on the GPU
     *
     * Space is deallocated only, when current data is present on GPU.
     */
    virtual void deallocate() const;

    virtual void bind() const;
    virtual void release() const;

    virtual int program() const;

    virtual void setUniform(std::string name, const QMatrix4x4 & value);
    virtual void setUniform(std::string name, const QMatrix3x3 & value);
    virtual void setUniform(std::string name, const QVector3D value);
    virtual void setUniform(std::string name, const QVector4D value);
    virtual void setUniform(std::string name, float value);
    virtual void setUniform(std::string name, int value);
    virtual void setUniform(std::string name, const QVector4D * values, int count);

private:
    std::vector<char> m_vertexSrc;
    std::vector<char> m_fragmentSrc;

    mutable unsigned int m_program;
    mutable unsigned int m_vertShader;
    mutable unsigned int m_fragShader;

    mutable bool m_isBound;
    mutable bool m_isOnGPU;
};

} //namespace terminus
