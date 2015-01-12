#include "levelgenerator.h"

#include <QVector2D>
#include <QVector4D>
#include <QDebug>

namespace terminus
{

float LevelGenerator::smoothstep(float e0, float e1, float x)
{
    x = std::min(std::max((x - e0)/(e1 - e0), 0.f), 1.f);
    return x*x*x*(x*(x*6.f - 15.f) + 10.f);
}

LevelGenerator::LevelGenerator()
: m_patchCountS(15)
, m_patchCountT(3)
, m_vertexCountS(64)
, m_vertexCountT(73)
, m_vertexWidth(1.f)
, m_vertexHeight(sqrt(3.f)/2.f)
, m_texGenerated(false)
, m_tracksGenerated(false)
, m_totalVertexCountS(m_patchCountS * (m_vertexCountS - 1) + 1)
, m_totalVertexCountT(m_patchCountT * (m_vertexCountT - 1) + 1)
, m_levelTexData(m_totalVertexCountS * m_totalVertexCountT * 4, 0.f)
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
    gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_totalVertexCountS, m_totalVertexCountT, 0, GL_RGBA, GL_FLOAT, m_levelTexData.data());
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

int LevelGenerator::patchCountS() const
{
    return m_patchCountS;
}

int LevelGenerator::patchCountT() const
{
    return m_patchCountT;
}

int LevelGenerator::vertexCountS() const
{
    return m_vertexCountS;
}

int LevelGenerator::vertexCountT() const
{
    return m_vertexCountT;
}

float LevelGenerator::vertexWidth() const
{
    return m_vertexWidth;
}

float LevelGenerator::vertexHeight() const
{
    return m_vertexHeight;
}

float LevelGenerator::patchWidth() const
{
    return static_cast<float>(m_vertexCountS - 1) * m_vertexWidth;
}

float LevelGenerator::patchHeight() const
{
    return static_cast<float>(m_vertexCountT - 1) * m_vertexHeight;
}

int LevelGenerator::totalVertexCountS() const
{
    return m_totalVertexCountS;
}

int LevelGenerator::totalVertexCountT() const
{
    return m_totalVertexCountT;
}

QPoint LevelGenerator::positionToVertexID(const QVector2D & pos) const
{
    int y = round(pos.y() / vertexHeight());
    int x = round((pos.x() - (y % 2 == 0 ? 0.f : 0.5f)) / vertexWidth());
    return QPoint(x, y);
}

QVector2D LevelGenerator::vertexIDToPosition(const QPoint & tex) const
{
    float x = tex.x() * vertexWidth() + (tex.y() % 2 == 0 ? 0.f : 0.5f);
    float y = tex.y() * vertexHeight();
    return QVector2D(x, y);
}

void LevelGenerator::generateLevelTracks(){
    if(m_tracksGenerated)
        return;

    std::vector<QVector2D> controlPoints;
    for(float x = 0.f; x < m_totalVertexCountS * m_vertexWidth; x += 16.f)
    {
        float z = static_cast<float>(rand() % 1000) / 1000.f * m_totalVertexCountT * m_vertexHeight;
        controlPoints.push_back(QVector2D(x, z));
    }

    m_trackSpline = std::unique_ptr<CatmullRomSpline>(new CatmullRomSpline(controlPoints));

    m_tracksGenerated = true;
}

void LevelGenerator::generateLevelTexImage(){
    if(m_texGenerated)
        return;
    generateLevelTracks();


    for(int iT = 0; iT < m_totalVertexCountT; iT++)
    {
        for(int iS = 0; iS < m_totalVertexCountS; iS++)
        {
            setLevelTexture(iS, iT, 0.f, 0.f, 0.f, 0.f);
        }
    }

    for(float param = 0.0; param <= m_trackSpline->length(); param += 0.025)
    {
        setTrackEnvironment(m_trackSpline->getPosition(param));
    }

    m_texGenerated = true;
}
void LevelGenerator::setLevelTexture(int s, int t, float dx, float dy, float dz, float w)
{
    if(s < 0 || t < 0 || s >= totalVertexCountS() || t >= totalVertexCountT())
        return;

    int texelIndex = (t * totalVertexCountS() + s) * 4;
    m_levelTexData[texelIndex]     = dx / 200.f + 0.5f;
    m_levelTexData[texelIndex + 1] = dy / 200.f + 0.5f;
    m_levelTexData[texelIndex + 2] = dz / 200.f + 0.5f;
    m_levelTexData[texelIndex + 3] = w;
}

QVector4D LevelGenerator::getLevelTexture(int s, int t)
{
    if(s < 0 || t < 0 || s >= totalVertexCountS() || t >= totalVertexCountT())
        return QVector4D();

    int texelIndex = (t * totalVertexCountS() + s) * 4;
    return QVector4D((m_levelTexData[texelIndex] - 0.5f) * 200,
                      (m_levelTexData[texelIndex + 1] - 0.5f) * 200,
                      (m_levelTexData[texelIndex + 2] - 0.5f) * 200,
                       m_levelTexData[texelIndex + 3]);
}

void LevelGenerator::setTrackEnvironment(const QVector2D & pointOnTrack)
{
    QPoint vid = positionToVertexID(pointOnTrack);
    int radius = 16;
    for(int iT = vid.y() - radius; iT <= vid.y() + radius; iT++)
    {
        for(int iS = vid.x() - radius; iS <= vid.x() + radius; iS++)
        {
            QVector2D point = vertexIDToPosition(QPoint(iS, iT));
            QVector4D displacement = getLevelTexture(iS, iT);
            float dist = pointOnTrack.distanceToPoint(point);
            float height = 1.f - smoothstep(0.f, 16.f, dist);
            setLevelTexture(iS, iT, 0.f, fmax(displacement.y(), height * 8.f), 0.f, 0.f);
        }
    }
}



}//namespace terminus
