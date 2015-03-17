#include "level.h"

#include <cmath>

#include <QVector2D>
#include <QVector4D>
#include <QDebug>

#include <util/mathutil.h>
#include <util/perlinnoise.h>

namespace terminus
{

Level::Level()
: Level(LevelConfiguration())
{
}

Level::Level(const LevelConfiguration & config)
: m_tracksGenerated(false)
, m_trackHeight(100.f)
, m_texGenerated(false)
{
    configure(config);
}

Level::~Level()
{
}

void Level::configure(const LevelConfiguration & config)
{
    m_config = config;
    m_terrainMapData = std::vector<GLfloat>(m_config.totalVertexCountS() * m_config.totalVertexCountT() * 4, 0.f);
    resetLevel();
}

const LevelConfiguration & Level::config() const
{
    return m_config;
}

void Level::generateLevel()
{
    generateTracks();
    generateTerrainMap();
}

void Level::resetLevel()
{
    m_noise.seed(m_config.seed());
    m_tracksGenerated = false;
    m_texGenerated = false;
}

std::vector<GLfloat> * Level::copyTerrainMapData() const
{
    return new std::vector<GLfloat>(m_terrainMapData);
}

Polyline * Level::createRightTrackCourse() const
{
    std::vector<QVector3D> points;
    if(m_rightTrack)
    {
        for(float param = 0.0; param <= m_rightTrack->length(); param += 0.1)
        {
            QVector2D pointInPlane = m_rightTrack->getPosition(param);
            points.push_back(QVector3D(pointInPlane.x(), trackHeight(), pointInPlane.y()));
        }
    }
    else
    {
        points.push_back(QVector3D(0.f, 0.f, m_config.totalHeight()));
        points.push_back(QVector3D(m_config.totalWidth(), 0.f, m_config.totalHeight()));
    }
    return new Polyline(points);
}

Polyline * Level::createLeftTrackCourse() const
{
    std::vector<QVector3D> points;
    if(m_leftTrack)
    {
        for(float param = 0.0; param <= m_leftTrack->length(); param += 0.1)
        {
            QVector2D pointInPlane = m_leftTrack->getPosition(param);
            points.push_back(QVector3D(pointInPlane.x(), trackHeight(), pointInPlane.y()));
        }
    }
    else
    {
        points.push_back(QVector3D(0.f, 0.f, 0.f));
        points.push_back(QVector3D(m_config.totalWidth(), 0.f, 0.f));
    }
    return new Polyline(points);
}

float Level::trackHeight() const
{
    return m_trackHeight;
}

void Level::generateTracks(){
    if(m_tracksGenerated)
        return;

    float minDist = 32.f,
            playerMinZ = m_config.totalHeight() / 2.f + 20.f,
            playerMaxZ = m_config.totalHeight() / 2.f + 60.f,
            enemyMinZ = m_config.totalHeight() / 2.f - 60.f,
            enemyMaxZ = m_config.totalHeight() / 2.f - 20.f;
    float xStep = 64.f,
            xBegin = m_config.totalHeight() / 2.f - xStep,
            xEnd = m_config.totalWidth() - m_config.totalHeight() / 2.f + xStep;

    std::vector<QVector2D> playerPoints;
    std::vector<QVector2D> enemyPoints;
    for(float x = xBegin; x < xEnd; x += xStep)
    {
        float zPlayer = MathUtil::mix(playerMinZ, playerMaxZ, m_noise.asymmetricRnd());
        float zEnemy = MathUtil::mix(enemyMinZ, enemyMaxZ, m_noise.asymmetricRnd());
        float distanceCorrect = - fmin(zPlayer - zEnemy - minDist, 0);
        playerPoints.push_back(QVector2D(x, zPlayer + distanceCorrect / 2.f));
        enemyPoints.push_back(QVector2D(x, zEnemy - distanceCorrect / 2.f));
    }

    m_rightTrack = std::unique_ptr<CatmullRomSpline>(new CatmullRomSpline(playerPoints));
    m_leftTrack = std::unique_ptr<CatmullRomSpline>(new CatmullRomSpline(enemyPoints));

    m_tracksGenerated = true;
}

void Level::generateTerrainMap(){
    if(m_texGenerated)
        return;
    generateTracks();

    for(int iT = 0; iT < m_config.totalVertexCountT(); iT++)
    {
        for(int iS = 0; iS < m_config.totalVertexCountS(); iS++)
        {
            QVector2D point = m_config.vertexIDToPosition(iS, iT);
            int i = tMapIndex(iS, iT);
            QVector2D d = terrainDisplacement(point.x(), point.y());
            tMapSetXYZ(i, d.x(), 0.f, d.y());
            tMapSetW(i, 0.f);
        }
    }

    setTrackEnvironment(*m_rightTrack);
    setTrackEnvironment(*m_leftTrack);

    for(int iT = 0; iT < m_config.totalVertexCountT(); iT++)
    {
        for(int iS = 0; iS < m_config.totalVertexCountS(); iS++)
        {
            QVector2D point = m_config.vertexIDToPosition(iS, iT);
            int i = tMapIndex(iS, iT);
            QVector3D offset = tMapGetXYZ(i);
            float h = terrainHeight(point.x() + offset.x(), point.y() + offset.z(), tMapGetW(i));
            tMapSetXYZ(i, offset.x(), h, offset.z());
        }
    }

    m_texGenerated = true;
}
QVector2D Level::terrainDisplacement(float x, float z){
    float scale = 0.5f,
            influence = 0.75f,
            dX = m_noise.noise(0, x * scale, z * scale) * influence * m_config.vertexWidth(),
            dZ = m_noise.noise(2, x * scale, z * scale) * influence * m_config.vertexHeight();
    return QVector2D(dX, dZ);
}

void Level::setTrackEnvironment(const CatmullRomSpline & track)
{
    float radiusBegin = 1.f,
            radiusEnd = 32.f;
    for(float param = 0.0; param <= track.length(); param += 0.025 * m_config.scale())
    {
        QVector2D trackPoint = track.getPosition(param);
        QPoint vid = m_config.positionToVertexID(trackPoint.x(), trackPoint.y());
        int radius = ceil(radiusEnd / m_config.scale());
        for(int iT = std::max(vid.y() - radius, 0); iT <= std::min(vid.y() + radius, m_config.totalVertexCountT()); iT++)
        {
            for(int iS = std::max(vid.x() - radius, 0); iS <= std::min(vid.x() + radius, m_config.totalVertexCountS()); iS++)
            {
                QVector2D point = m_config.vertexIDToPosition(iS, iT);
                int i = tMapIndex(iS, iT);
                QVector3D offset = tMapGetXYZ(i);
                float fTrack = tMapGetW(i);

                float dist = MathUtil::distance(point.x() + offset.x(), point.y() + offset.z(),
                                                trackPoint.x(), trackPoint.y());
                fTrack = fmax(1.f - MathUtil::smoothstep(radiusBegin, radiusEnd, dist), fTrack);
                tMapSetW(i, fTrack);
            }
        }
    }
}

float Level::terrainHeight(float x, float z, float fTrack){
    float mountainBase = 140.f,
            mountainDiff = 120.f,
            valleyBase = 30.f,
            valleyDiff = 60.f,
            landscapeFreq = 0.01f;
    float rockBegin = 60.f,
            rockEnd = 200.f,
            rockFreq = 0.05f,
            rockMinInfluence = 5.f,
            rockMaxInfluence = 20.f;
    float groundTrackInfluence = 0.1f,
            groundTerrainInfluence = 2.f,
            groundRockInfluence = 5.f,
            groundFreq = 1.f;
    float borderBegin = 10.f,
            borderEnd = 75.f;

    float center = m_config.totalHeight() / 2.f,
            dXBorder = x - MathUtil::clamp(center, m_config.totalWidth() - center, x),
            dZBorder = z - center,
            fToBorder = MathUtil::smoothstep(borderBegin, borderEnd, sqrt(dXBorder * dXBorder + dZBorder * dZBorder));

    float height = MathUtil::mix(valleyBase, mountainBase, fToBorder)
                    + m_noise.noise(1, landscapeFreq * x, landscapeFreq * z)
                    * MathUtil::mix(valleyDiff, mountainDiff, fToBorder);

    float fRockyness = MathUtil::smoothstep(rockBegin, rockEnd, height);
    float rockOffset = m_noise.noise(1, rockFreq * x, rockFreq * z)
                        * MathUtil::mix(rockMinInfluence, rockMaxInfluence, fRockyness);

    float groundOffset = m_noise.noise(1, groundFreq * x, groundFreq * z)
                * MathUtil::mix(MathUtil::mix(groundTerrainInfluence, groundRockInfluence, fRockyness), groundTrackInfluence, fTrack);
    return MathUtil::mix(height + rockOffset, trackHeight(), fTrack) + groundOffset;
}

int Level::tMapIndex(int s, int t) const
{
    return (t * m_config.totalVertexCountS() + s) * 4;
}

void Level::tMapSetXYZ(int i, float dx, float dy, float dz)
{
    m_terrainMapData[i] = dx / m_config.scale() + 0.5f;
    m_terrainMapData[i + 1] = dy / m_config.scale() / 200.f;
    m_terrainMapData[i + 2] = dz / m_config.scale() + 0.5f;
}
QVector3D Level::tMapGetXYZ(int i) const
{
    return QVector3D( (m_terrainMapData[i] - 0.5f) * m_config.scale(),
                        m_terrainMapData[i + 1] * 200.f * m_config.scale(),
                       (m_terrainMapData[i + 2] - 0.5f) * m_config.scale());
}
void Level::tMapSetW(int i, float w)
{
    m_terrainMapData[i + 3] = w;
}
float Level::tMapGetW(int i) const
{
    return m_terrainMapData[i + 3];
}



}//namespace terminus
