#include "program.h"

#include <QDebug>
#include <QOpenGLShaderProgram>

Program::Program()
: m_program(nullptr)
, m_isBound(false)
, m_isOnGPU(false)
{
}
Program::Program(std::string vertexSrc, std::string fragmentSrc)
: m_program(nullptr)
, m_isBound(false)
, m_isOnGPU(false)
, m_vertexSrc(vertexSrc)
, m_fragmentSrc(fragmentSrc)
{
}

Program::~Program()
{
    deallocate();
}

void Program::allocate()
{
    if(m_isOnGPU)
        return;

    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, m_vertexSrc.c_str());
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, m_fragmentSrc.c_str());

    m_isOnGPU = true;
}
void Program::deallocate()
{
    if(!m_isOnGPU)
        return;

    if(m_program)
    {
        if(m_isBound)
        {
            m_program->release();
        }
        delete m_program;
        m_program = nullptr;
    }

    m_isOnGPU = false;
}

QOpenGLShaderProgram & Program::program()
{
    return *m_program;
}

void Program::bind()
{
    allocate();
    if(m_isBound)
    {
        return;
    }
    m_program->bind();
    m_isBound = true;
}
void Program::release()
{
    if(!m_isBound)
    {
        return;
    }
    m_program->release();
    m_isBound = false;
}

void Program::bindAttributeLocation(std::string name, int location)
{
    m_program->bindAttributeLocation(name.c_str(), location);
}

void Program::setUniform(std::string name, const QMatrix4x4 & value)
{
    int location = 0;
    if((location = m_program->uniformLocation(name.c_str())) >= 0)
    {
        m_program->setUniformValue(location, value);
    }
}
void Program::setUniform(std::string name, const QMatrix3x3 & value)
{
    int location = 0;
    if((location = m_program->uniformLocation(name.c_str())) >= 0)
    {
        m_program->setUniformValue(location, value);
    }
}
void Program::setUniform(std::string name, const QVector3D value)
{
    int location = 0;
    if((location = m_program->uniformLocation(name.c_str())) >= 0)
    {
        m_program->setUniformValue(location, value);
    }
}
void Program::setUniform(std::string name, const QVector4D value)
{
    int location = 0;
    if((location = m_program->uniformLocation(name.c_str())) >= 0)
    {
        m_program->setUniformValue(location, value);
    }
}
void Program::setUniform(std::string name, float value)
{
    int location = 0;
    if((location = m_program->uniformLocation(name.c_str())) >= 0)
    {
        m_program->setUniformValue(location, value);
    }
}
void Program::setUniform(std::string name, int value)
{
    int location = 0;
    if((location = m_program->uniformLocation(name.c_str())) >= 0)
    {
        m_program->setUniformValue(location, value);
    }
}
