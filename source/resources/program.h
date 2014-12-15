#pragma once

#include <string>

#include <QMatrix4x4>
#include <QMatrix3x3>
#include <QVector4D>
#include <QVector3D>

class QOpenGLShaderProgram;

class Program
{
public:
    Program();
    Program(std::string vertexSrc, std::string fragmentSrc);
    virtual ~Program();

    virtual void allocate();
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
private:
    std::string m_vertexSrc;
    std::string m_fragmentSrc;

    QOpenGLShaderProgram * m_program;

    bool m_isBound;
    bool m_isOnGPU;
};
