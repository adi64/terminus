#include "levelgenerator.h"

#include <QVector2D>
#include <QVector4D>
#include <QDebug>

#include "mathutil.h"
#include "perlinnoise.h"

namespace terminus
{



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
    for(float x = 0.f; x < m_totalVertexCountS * m_vertexWidth; x += 64.f)
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

    m_noiseX = std::unique_ptr<PerlinNoise>(new PerlinNoise()); //use explicit seed?
    m_noiseY = std::unique_ptr<PerlinNoise>(new PerlinNoise()); //use explicit seed?
    m_noiseZ = std::unique_ptr<PerlinNoise>(new PerlinNoise()); //use explicit seed?

    float scale = 0.1f;

    for(int iT = 0; iT < m_totalVertexCountT; iT++)
    {
        for(int iS = 0; iS < m_totalVertexCountS; iS++)
        {
            setTexComponent(iS, iT, 3, 0.f);
            float dispScale = 0.5f;
            float dX = m_noiseX->noise(iS * dispScale, iT * dispScale) * 0.125f,
                    dZ = m_noiseZ->noise(iS * dispScale, iT * dispScale) * 0.125f;
            setTexComponent(iS, iT, 0, dX);
            setTexComponent(iS, iT, 2, dZ);
        }
    }
    for(float param = 0.0; param <= m_trackSpline->length(); param += 0.025)
    {
        setTrackEnvironment(m_trackSpline->getPosition(param));
    }
    for(int iT = 0; iT < m_totalVertexCountT; iT++)
    {
        for(int iS = 0; iS < m_totalVertexCountS; iS++)
        {
            QVector2D point = vertexIDToPosition(QPoint(iS, iT));
            float dX = getTexComponent(iS, iT, 0),
                    dZ = getTexComponent(iS, iT, 2),
                    fTrack = getTexComponent(iS, iT, 3);
            point += QVector2D(dX, dZ);
            float h = terrainHeight(point.x(), point.y(), fTrack);
            setTexComponent(iS, iT, 1, h);
        }
    }

    m_texGenerated = true;
}

void LevelGenerator::setTexComponent(int s, int t, int component, float value)
{
    if(s < 0 || t < 0 || s >= totalVertexCountS() || t >= totalVertexCountT() || component < 0 || component >= 4)
        return;

    int texelIndex = (t * totalVertexCountS() + s) * 4 + component;
    if(component == 3)
    {
        m_levelTexData[texelIndex] = value;
    }
    else
    {
        m_levelTexData[texelIndex] = value / 200.f + 0.5f;
    }
}

float LevelGenerator::getTexComponent(int s, int t, int component)
{
    if(s < 0 || t < 0 || s >= totalVertexCountS() || t >= totalVertexCountT() || component < 0 || component >= 4)
        return 0.f;

    int texelIndex = (t * totalVertexCountS() + s) * 4 + component;
    if(component == 3)
    {
        return m_levelTexData[texelIndex];
    }
    else
    {
        return (m_levelTexData[texelIndex] - 0.5f) * 200.f;
    }
}

float LevelGenerator::terrainHeight(float x, float z, float fTrack){
    float trackHeight = 10.f;
    float zMid = totalVertexCountT() * vertexHeight() / 2.f;
    float fToBorder = MathUtil::smoothstep(36.f, 73.f, fabs(z - zMid));
    float scale = 0.025f; //basic structure
    float height = fToBorder * 20.f + m_noiseZ->noise(scale * x, scale * z) * (40.f + fToBorder * 20.f);
    float fRockyness = MathUtil::smoothstep(20.f, 40.f, height);
    scale = 0.2; //details
    height += m_noiseZ->noise(scale * x, scale * z) * (1.f + fRockyness * 9.f);
    return MathUtil::mix(height, trackHeight, fTrack);
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
            float dX = getTexComponent(iS, iT, 0),
                    dZ = getTexComponent(iS, iT, 2);
            float fTrack = getTexComponent(iS, iT, 3);

            float dist = MathUtil::distance(point.x() + dX, point.y() + dZ,
                                            pointOnTrack.x(), pointOnTrack.y());
            fTrack = fmax(1.f - MathUtil::smoothstep(0.f, 16.f, dist), fTrack);
            setTexComponent(iS, iT, 3, fTrack);
        }
    }
}



}//namespace terminus
