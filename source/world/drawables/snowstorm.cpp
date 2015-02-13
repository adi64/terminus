#include "snowstorm.h"

#include <QDebug>
#include <iostream>

#include <world/scene.h>
#include <resources/resourcemanager.h>
#include <resources/geometry.h>
#include <resources/program.h>


namespace terminus
{

SnowStorm::SnowStorm(std::shared_ptr<Scene> scene)
    : AbstractGraphicsObject(scene)
    , m_width(2048)
    , m_flakes(width() * width())
    , m_noise()
    , m_initialized(false)
{
    m_program = ResourceManager::getInstance()->getProgram("snowstorm");
    m_geometry = ResourceManager::getInstance()->getGeometry("base_ScreenQuad");
    initializeFlakes();
}

SnowStorm::~SnowStorm()
{
}

void SnowStorm::update(int elapsedMilliseconds)
{
}

void SnowStorm::preRender(QOpenGLFunctions & gl, Program & program) const
{
    if(!m_initialized)
    {
        initializeTextures(gl);
    }
    gl.glActiveTexture(GL_TEXTURE0);
    gl.glBindTexture(GL_TEXTURE_2D, m_texture);
}

void SnowStorm::postRender(QOpenGLFunctions & gl, Program & program) const
{
    gl.glActiveTexture(GL_TEXTURE0);
    gl.glBindTexture(GL_TEXTURE_2D, 0);
}

void SnowStorm::initializeFlakes()
{
    for(unsigned int x = 0; x < flakes().size() / width(); x++)
    {
        for(unsigned int y = 0; y < flakes().size() / width(); y++)
        {
            flakes()[x * width() + y] = m_noise.noise(0, static_cast<float>(x)* 0.01f, static_cast<float>(y) * 0.01f);       //0.01f is used to shift the position from gradient position
        }
    }
}

void SnowStorm::initializeTextures(QOpenGLFunctions &gl) const
{
    m_texture = -1;
    gl.glGenTextures(1, &m_texture);
    gl.glBindTexture(GL_TEXTURE_2D, m_texture);

    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, m_width, m_width, 0, GL_ALPHA, GL_FLOAT, m_flakes.data());

    gl.glBindTexture(GL_TEXTURE_2D, 0);

    m_initialized = true;
}

int SnowStorm::width()
{
    return m_width;
}

std::vector<float> & SnowStorm::flakes()
{
    return m_flakes;
}

}
