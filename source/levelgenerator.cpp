#include "levelgenerator.h"

namespace terminus
{

LevelGenerator::LevelGenerator()
: m_patchesX(15)
, m_patchesZ(3)
, m_patchVertX(64)
, m_patchVertZ(73)
, m_patchLenVertX(1.f)
, m_patchLenVertZ(sqrt(3.f)/2.f)
, m_texGenerated(false)
, m_tracksGenerated(false)
, m_levelTexWidth(m_patchesX * (m_patchVertX - 1) + 1)
, m_levelTexHeight(m_patchesZ * (m_patchVertZ - 1) + 1)
, m_levelTexData(m_levelTexWidth * m_levelTexHeight * 4, 0.f)
, m_isTexAllocated(false)
{
}

LevelGenerator::~LevelGenerator()
{
    //do not deallocate texture on GPU
}

void LevelGenerator::generateLevel(){
    m_tracksGenerated = false;
    m_texGenerated = false;
    generateLevelTracks();
    generateLevelTexImage();
}

void LevelGenerator::bindLevelTexTo(QOpenGLFunctions & gl, GLenum unit) const
{
    if(allocateLevelTex(gl))
    {
        gl.glActiveTexture(unit);
        gl.glBindTexture(GL_TEXTURE_2D, m_levelTex);
    }
}

void LevelGenerator::releaseLevelTexFrom(QOpenGLFunctions & gl, GLenum unit) const
{
    gl.glActiveTexture(unit);
    gl.glBindTexture(GL_TEXTURE_2D, 0);
}

bool LevelGenerator::allocateLevelTex(QOpenGLFunctions & gl) const
{
    if(m_isTexAllocated)
        return true;

    if(!m_texGenerated)
        return false;

    gl.glGenTextures(1, &m_levelTex);

    gl.glBindTexture(GL_TEXTURE_2D, m_levelTex);
    gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_levelTexWidth, m_levelTexHeight, 0, GL_RGBA, GL_FLOAT, m_levelTexData.data());
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    gl.glBindTexture(GL_TEXTURE_2D, 0);

    m_isTexAllocated = true;
    return m_isTexAllocated;
}

void LevelGenerator::deallocateLevelTex(QOpenGLFunctions & gl) const
{
    if(!m_isTexAllocated)
        return;

    gl.glDeleteTextures(1, &m_levelTex);

    m_isTexAllocated = false;
}

int LevelGenerator::patchesX() const
{
    return m_patchesX;
}

int LevelGenerator::patchesZ() const
{
    return m_patchesZ;
}

int LevelGenerator::patchVerticesX() const
{
    return m_patchVertX;
}

int LevelGenerator::patchVerticesZ() const
{
    return m_patchVertZ;
}

float LevelGenerator::patchVertLengthX() const
{
    return m_patchLenVertX;
}

float LevelGenerator::patchVertLengthZ() const
{
    return m_patchLenVertZ;
}

float LevelGenerator::patchLengthX() const
{
    return static_cast<float>(m_patchVertX - 1) * m_patchLenVertX;
}

float LevelGenerator::patchLengthZ() const
{
    return static_cast<float>(m_patchVertZ - 1) * m_patchLenVertZ;
}

int LevelGenerator::levelTexWidth() const
{
    return m_levelTexWidth;
}

int LevelGenerator::levelTexHeight() const
{
    return m_levelTexHeight;
}

void LevelGenerator::generateLevelTracks(){
    if(m_tracksGenerated)
        return;

    m_tracksGenerated = true;
}

void LevelGenerator::generateLevelTexImage(){
    if(m_texGenerated)
        return;
    generateLevelTracks();

    for(int iV = 0; iV < m_levelTexHeight; iV++)
    {
        for(int iU = 0; iU < m_levelTexWidth; iU++)
        {
            int pixelIndex = (iV * m_levelTexWidth + iU) * 4;
            m_levelTexData[pixelIndex]     = (static_cast<GLfloat>(rand()%16) / 16.f)           / 200.f + 0.5; //x displacement
            m_levelTexData[pixelIndex + 1] = (sin(sqrt(static_cast<GLfloat>(iU*iU + iV*iV)) / 16.f) * 10.0) / 200.f + 0.5; //height displacement
            m_levelTexData[pixelIndex + 2] = (static_cast<GLfloat>(rand()%16) / 16.f)           / 200.f + 0.5; //z displacement
            m_levelTexData[pixelIndex + 3] = 0.f; //will be used later
        }
    }

    m_texGenerated = true;
}

}//namespace terminus
