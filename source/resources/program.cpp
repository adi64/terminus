#include "program.h"

#include <GLES3/gl3.h>

namespace terminus
{

Program::Program()
: m_program(nullptr)
, m_isBound(false)
, m_isOnGPU(false)
{
}
Program::Program(std::string vertexSrc, std::string fragmentSrc)
: m_vertexSrc(vertexSrc)
, m_fragmentSrc(fragmentSrc)
, m_program(nullptr)
, m_isBound(false)
, m_isOnGPU(false)
{
}

Program::~Program()
{
    deallocate();
}

void Program::allocate() const
{
    if(m_isOnGPU)
        return;

    m_program = glCreateProgram();

    m_vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLchar * sourceString = m_vertexSrc.c_str();
    GLint sourceStringLength =  m_vertexSrc.size();
    glShaderSource(m_vertShader, 1, &sourceString, &sourceStringLength);
    glCompileShader(m_vertShader);
    glAttachShader(m_program, m_vertShader);

    m_fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    sourceString = m_fragmentSrc.c_str();
    sourceStringLength = m_fragmentSrc.size();
    glShaderSource(m_fragShader, 1, &sourceString, &sourceStringLength);
    glCompileShader(m_fragShader);
    glAttachShader(m_program, m_fragShader);

    glLinkProgram(m_program);

    //TODO-LW: add error detection

    m_isOnGPU = true;
}
void Program::deallocate() const
{
    if(!m_isOnGPU)
        return;

    glDeleteProgram(m_program);
    glDeleteShader(m_vertShader);
    glDeleteShader(m_fragShader);

    m_isOnGPU = false;
}

int Program::program() const
{
    allocate();
    return m_program;
}

void Program::bind() const
{
    glUseProgram(program());
    m_isBound = true;
}
void Program::release() const
{
    if(!m_isBound)
        return;

    glUseProgram(0);
    m_isBound = false;
}

void Program::setUniform(std::string name, const QMatrix4x4 & value)
{
    int location = 0;
    if((location = glGetUniformLocation(program(), name.c_str())) >= 0)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, value.constData());
    }
}
void Program::setUniform(std::string name, const QMatrix3x3 & value)
{
    int location = 0;
    if((location = glGetUniformLocation(program(), name.c_str())) >= 0)
    {
        glUniformMatrix3fv(location, 1, GL_FALSE, value.constData());
    }
}
void Program::setUniform(std::string name, const QVector3D value)
{
    int location = 0;
    if((location = glGetUniformLocation(program(), name.c_str())) >= 0)
    {
        glUniform3fv(location, 1, reinterpret_cast<GLfloat *>(value));
    }
}
void Program::setUniform(std::string name, const QVector4D value)
{
    int location = 0;
    if((location = glGetUniformLocation(program(), name.c_str())) >= 0)
    {
        glUniform4fv(location, 1, reinterpret_cast<GLfloat *>(value));
    }
}
void Program::setUniform(std::string name, float value)
{
    int location = 0;
    if((location = glGetUniformLocation(program(), name.c_str())) >= 0)
    {
        glUniform1f(location, value);
    }
}
void Program::setUniform(std::string name, int value)
{
    int location = 0;
    if((location = glGetUniformLocation(program(), name.c_str())) >= 0)
    {
        glUniform1i(location, value);
    }
}
void Program::setUniform(std::string name, const QVector4D * values, int count)
{
    int location = 0;
    if((location = glGetUniformLocation(program(), name.c_str())) >= 0)
    {
        glUniform4fv(location, count, reinterpret_cast<GLfloat *>(values));
        m_program->setUniformValueArray(location, values, count);
    }
}

} //namespace terminus
