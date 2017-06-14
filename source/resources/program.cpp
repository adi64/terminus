#include "program.h"

#include <glincl.h>

#include <util/gldebug.h>

namespace terminus
{

Program::Program()
: m_program(0)
, m_isBound(false)
, m_isOnGPU(false)
{
}
Program::Program(std::vector<char> && vertexSrc, std::vector<char> && fragmentSrc)
: m_vertexSrc(vertexSrc)
, m_fragmentSrc(fragmentSrc)
, m_program(0)
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

    printGlError(__FILE__, __LINE__);

    m_program = gl.glCreateProgram();
    printGlError(__FILE__, __LINE__);

    m_vertShader = gl.glCreateShader(GL_VERTEX_SHADER);
    const char * vertSourceString = m_vertexSrc.data();
    int vertSourceStringLength = m_vertexSrc.size();
    gl.glShaderSource(m_vertShader, 1, &vertSourceString, &vertSourceStringLength);
    gl.glCompileShader(m_vertShader);
    GLint compiled;
    gl.glGetShaderiv(m_vertShader, GL_COMPILE_STATUS, &compiled);
    if(!compiled)
    {
        qDebug() << "Compile Error";

        GLint logSize = 0;
        gl.glGetShaderiv(m_vertShader, GL_INFO_LOG_LENGTH, &logSize);
        std::vector<GLchar> errorLog(logSize);
        gl.glGetShaderInfoLog(m_vertShader, logSize, &logSize, &errorLog[0]);

        qDebug() << QString::fromUtf8(errorLog.data());
    }
    gl.glAttachShader(m_program, m_vertShader);

    printGlError(__FILE__, __LINE__);

    m_fragShader = gl.glCreateShader(GL_FRAGMENT_SHADER);
    const char * fragSourceString = m_fragmentSrc.data();
    int fragSourceStringLength = m_fragmentSrc.size();
    gl.glShaderSource(m_fragShader, 1, &fragSourceString, &fragSourceStringLength);
    gl.glCompileShader(m_fragShader);
    gl.glGetShaderiv(m_fragShader, GL_COMPILE_STATUS, &compiled);
    if(!compiled)
    {
        qDebug() << "Compile Error";

        GLint logSize = 0;
        gl.glGetShaderiv(m_fragShader, GL_INFO_LOG_LENGTH, &logSize);
        std::vector<GLchar> errorLog(logSize);
        gl.glGetShaderInfoLog(m_fragShader, logSize, &logSize, &errorLog[0]);

        qDebug() << QString::fromUtf8(errorLog.data()) << "\n";
    }
    gl.glAttachShader(m_program, m_fragShader);

    printGlError(__FILE__, __LINE__);

    gl.glLinkProgram(m_program);

    //TODO-LW: add error detection
    printGlError(__FILE__, __LINE__);

    m_isOnGPU = true;
}
void Program::deallocate() const
{
    if(!m_isOnGPU)
        return;
    printGlError(__FILE__, __LINE__);
    gl.glDeleteProgram(m_program);
    gl.glDeleteShader(m_vertShader);
    gl.glDeleteShader(m_fragShader);
    printGlError(__FILE__, __LINE__);
    m_isOnGPU = false;
}

int Program::program() const
{
    printGlError(__FILE__, __LINE__);
    allocate();
    printGlError(__FILE__, __LINE__);
    return m_program;
}

void Program::bind() const
{
    printGlError(__FILE__, __LINE__);
    gl.glUseProgram(program());
    printGlError(__FILE__, __LINE__);
    m_isBound = true;
}
void Program::release() const
{
    if(!m_isBound)
        return;

    printGlError(__FILE__, __LINE__);
    gl.glUseProgram(0);
    printGlError(__FILE__, __LINE__);
    m_isBound = false;
}

void Program::setUniform(std::string name, const QMatrix4x4 & value, bool suppressErrors /*=false*/)
{
    printGlError(__FILE__, __LINE__);
    bind();
    int location = 0;
    if((location = gl.glGetUniformLocation(program(), name.c_str())) >= 0)
    {
        gl.glUniformMatrix4fv(location, 1, GL_FALSE, value.constData());
    }
    else
    {
        if (!suppressErrors)
            qDebug() << __FILE__ << " at " << __LINE__ << ": " << "Could not get location for " << QString::fromStdString(name);
    }
    printGlError(__FILE__, __LINE__);
}
void Program::setUniform(std::string name, const QMatrix3x3 & value, bool suppressErrors /*=false*/)
{
    printGlError(__FILE__, __LINE__);
    bind();
    int location = 0;
    if((location = gl.glGetUniformLocation(program(), name.c_str())) >= 0)
    {
        gl.glUniformMatrix3fv(location, 1, GL_FALSE, value.constData());
    }
    else
    {
        if (!suppressErrors)
            qDebug() << __FILE__ << " at " << __LINE__ << ": " << "Could not get location for " << QString::fromStdString(name);
    }
    printGlError(__FILE__, __LINE__);
}
void Program::setUniform(std::string name, const QVector3D value, bool suppressErrors /*=false*/)
{
    printGlError(__FILE__, __LINE__);
    bind();
    int location = 0;
    if((location = gl.glGetUniformLocation(program(), name.c_str())) >= 0)
    {
        gl.glUniform3fv(location, 1, reinterpret_cast<const GLfloat *>(&value));
    }
    else
    {
        if (!suppressErrors)
            qDebug() << __FILE__ << " at " << __LINE__ << ": " << "Could not get location for " << QString::fromStdString(name);
    }
    printGlError(__FILE__, __LINE__);
}
void Program::setUniform(std::string name, const QVector4D value, bool suppressErrors /*=false*/)
{
    printGlError(__FILE__, __LINE__);
    bind();
    int location = 0;
    if((location = gl.glGetUniformLocation(program(), name.c_str())) >= 0)
    {
        gl.glUniform4fv(location, 1, reinterpret_cast<const GLfloat *>(&value));
    }
    else
    {
        if (!suppressErrors)
            qDebug() << __FILE__ << " at " << __LINE__ << ": " << "Could not get location for " << QString::fromStdString(name);
    }
    printGlError(__FILE__, __LINE__);
}
void Program::setUniform(std::string name, float value, bool suppressErrors /*=false*/)
{
    printGlError(__FILE__, __LINE__);
    bind();
    int location = 0;
    if((location = gl.glGetUniformLocation(program(), name.c_str())) >= 0)
    {
        gl.glUniform1f(location, value);
    }
    else
    {
        if (!suppressErrors)
            qDebug() << __FILE__ << " at " << __LINE__ << ": " << "Could not get location for " << QString::fromStdString(name);
    }
    printGlError(__FILE__, __LINE__);
}
void Program::setUniform(std::string name, int value, bool suppressErrors /*=false*/)
{
    printGlError(__FILE__, __LINE__);
    bind();
    int location = 0;
    if((location = gl.glGetUniformLocation(program(), name.c_str())) >= 0)
    {
        gl.glUniform1i(location, value);
    }
    else
    {
        if (!suppressErrors)
            qDebug() << __FILE__ << " at " << __LINE__ << ": " << "Could not get location for " << QString::fromStdString(name);
    }
    printGlError(__FILE__, __LINE__);
}
void Program::setUniform(std::string name, const QVector4D * values, int count, bool suppressErrors /*=false*/)
{
    printGlError(__FILE__, __LINE__);
    //name.append("[0]");
    bind();
    int location = 0;
    if((location = gl.glGetUniformLocation(program(), name.c_str())) >= 0)
    {
        gl.glUniform4fv(location, count, reinterpret_cast<const GLfloat *>(values));
    }
    else
    {
        if (!suppressErrors)
            qDebug() << __FILE__ << " at " << __LINE__ << ": " << "Could not get location for " << QString::fromStdString(name);
    }
    printGlError(__FILE__, __LINE__);
}

} //namespace terminus
