#include "level.h"

#include <QVector2D>
#include <QVector4D>
#include <QDebug>

#include "mathutil.h"
#include "perlinnoise.h"

namespace terminus
{

Level::Level()
: m_patchCountS(64)
, m_patchCountT(5)
, m_vertexCountS(64)
, m_vertexCountT(73)
, m_vertexWidth(1.f)
, m_vertexHeight(sqrt(3.f)/2.f)
, m_totalVertexCountS(m_patchCountS * (m_vertexCountS - 1) + 1)
, m_totalVertexCountT(m_patchCountT * (m_vertexCountT - 1) + 1)
, m_noiseX(/*seed*/)
, m_noiseY(/*seed*/)
, m_noiseZ(/*seed*/)
, m_tracksGenerated(false)
, m_trackHeight(10.f)
, m_texGenerated(false)
, m_terrainMapData(m_totalVertexCountS * m_totalVertexCountT * 4, 0.f)
{
}

Level::~Level()
{
}

void Level::generateLevel(){
    m_tracksGenerated = false;
    m_texGenerated = false;
    generateTracks();
    generateTerrainMap();
}

int Level::patchCountS() const
{
    return m_patchCountS;
}
int Level::patchCountT() const
{
    return m_patchCountT;
}
int Level::vertexCountS() const
{
    return m_vertexCountS;
}
int Level::vertexCountT() const
{
    return m_vertexCountT;
}
float Level::vertexWidth() const
{
    return m_vertexWidth;
}
float Level::vertexHeight() const
{
    return m_vertexHeight;
}
float Level::patchWidth() const
{
    return static_cast<float>(vertexCountS() - 1) * vertexWidth();
}
float Level::patchHeight() const
{
    return static_cast<float>(vertexCountT() - 1) * vertexHeight();
}
int Level::totalVertexCountS() const
{
    return m_totalVertexCountS;
}
int Level::totalVertexCountT() const
{
    return m_totalVertexCountT;
}
int Level::totalWidth() const
{
    return totalVertexCountS() * vertexWidth();
}
int Level::totalHeight() const
{
    return totalVertexCountT() * vertexHeight();
}
QPoint Level::positionToVertexID(float x, float z) const
{
    int t = round(z / vertexHeight());
    int s = round( x / vertexWidth() - (t % 2 == 0 ? 0.5f : 0.f));
    return QPoint(s, t);
}
QPoint Level::positionToPatchID(float x, float z) const
{
    int t = round(z / patchHeight());
    int s = round(x / patchWidth());
    return QPoint(s, t);
}
QVector2D Level::vertexIDToPosition(int s, int t) const
{
    float x = (static_cast<float>(s) + (t % 2 == 0 ? 0.5f : 0.f)) * vertexWidth();//TODO find out why that works
    float z = static_cast<float>(t) * vertexHeight();
    return QVector2D(x, z);
}

const void * Level::terrainMapData() const
{
    //generateTerrainMap(); TODO mechanism to assure previous generateLevel() call

    return m_terrainMapData.data();
}

std::unique_ptr<Polyline> Level::playerTrack()
{
    generateTracks();

    std::vector<QVector3D> points;
    for(float param = 0.0; param <= m_playerTrack->length(); param += 0.1)
    {
        QVector2D pointInPlane = m_playerTrack->getPosition(param);
        points.push_back(QVector3D(pointInPlane.x(), trackHeight(), pointInPlane.y()));
    }
    return std::unique_ptr<Polyline>(new Polyline(points));
}

std::unique_ptr<Polyline> Level::enemyTrack()
{
    generateTracks();

    std::vector<QVector3D> points;
    for(float param = 0.0; param <= m_enemyTrack->length(); param += 0.1)
    {
        QVector2D pointInPlane = m_enemyTrack->getPosition(param);
        points.push_back(QVector3D(pointInPlane.x(), trackHeight(), pointInPlane.y()));
    }
    return std::unique_ptr<Polyline>(new Polyline(points));
}

float Level::trackHeight()
{
    return m_trackHeight;
}

void Level::generateTracks(){
    if(m_tracksGenerated)
        return;

    float playerBaseZ = totalHeight() / 2.f + patchHeight() / 2.f,
            playerDiffZ = patchHeight();
    float enemyMinDist = 16.f,
            enemyDiffZ = patchHeight();
    float xStep = 64.f,
            xBegin = totalHeight() / 2.f,
            xEnd = totalWidth() - totalHeight() / 2.f;

    std::vector<QVector2D> playerPoints;
    std::vector<QVector2D> enemyPoints;
    for(float x = xBegin - xStep; x < xEnd + xStep; x += xStep)
    {
        float zPlayer = playerBaseZ + m_noiseX.symmetricRnd() * playerDiffZ;
        float zEnemy = zPlayer - (enemyMinDist + m_noiseX.asymmetricRnd() * enemyDiffZ);
        playerPoints.push_back(QVector2D(x, zPlayer));
        enemyPoints.push_back(QVector2D(x, zEnemy));
    }

    m_playerTrack = std::unique_ptr<CatmullRomSpline>(new CatmullRomSpline(playerPoints));
    m_enemyTrack = std::unique_ptr<CatmullRomSpline>(new CatmullRomSpline(enemyPoints));

    m_tracksGenerated = true;
}

void Level::generateTerrainMap(){
    if(m_texGenerated)
        return;
    generateTracks();

    for(int iT = 0; iT < m_totalVertexCountT; iT++)
    {
        for(int iS = 0; iS < m_totalVertexCountS; iS++)
        {
            QVector2D point = vertexIDToPosition(iS, iT);
            int i = tMapIndex(iS, iT);
            QVector2D d = terrainDisplacement(point.x(), point.y());
            m_terrainMapData[i] = tMapEncode(d.x());
            m_terrainMapData[i + 1] = tMapEncode(0.f);
            m_terrainMapData[i + 2] = tMapEncode(d.y());
            m_terrainMapData[i + 3] = 0.f;
        }
    }

    setTrackEnvironment(*m_playerTrack);
    setTrackEnvironment(*m_enemyTrack);

    for(int iT = 0; iT < m_totalVertexCountT; iT++)
    {
        for(int iS = 0; iS < m_totalVertexCountS; iS++)
        {
            QVector2D point = vertexIDToPosition(iS, iT);
            int i = tMapIndex(iS, iT);
            float x = point.x() + tMapDecode(m_terrainMapData[i]),
                    z = point.y() + tMapDecode(m_terrainMapData[i + 2]),
                    fTrack = m_terrainMapData[i + 3];
            float h = terrainHeight(x, z, fTrack);
            m_terrainMapData[i + 1] = tMapEncode(h);
        }
    }

    m_texGenerated = true;
}
QVector2D Level::terrainDisplacement(float x, float z){
    float scale = 0.5f,
            influence = 0.4,
            dX = m_noiseX.noise(x * scale, z * scale) * influence,
            dZ = m_noiseZ.noise(x * scale, z * scale) * influence;
    return QVector2D(dX, dZ);
}

void Level::setTrackEnvironment(const CatmullRomSpline & track)
{
    float radiusBegin = 1.f,
            radiusEnd = 16.f;
    for(float param = 0.0; param <= track.length(); param += 0.025)
    {
        QVector2D trackPoint = track.getPosition(param);
        QPoint vid = positionToVertexID(trackPoint.x(), trackPoint.y());
        int radius = 16;
        for(int iT = vid.y() - radius; iT <= vid.y() + radius; iT++)
        {
            for(int iS = vid.x() - radius; iS <= vid.x() + radius; iS++)
            {
                QVector2D point = vertexIDToPosition(iS, iT);
                int i = tMapIndex(iS, iT);
                float fTrack = m_terrainMapData[i + 3],
                        dX = tMapDecode(m_terrainMapData[i]),
                        dZ = tMapDecode(m_terrainMapData[i + 2]);

                float dist = MathUtil::distance(point.x() + dX, point.y() + dZ,
                                                trackPoint.x(), trackPoint.y());
                fTrack = fmax(1.f - MathUtil::smoothstep(radiusBegin, radiusEnd, dist), fTrack);
                m_terrainMapData[i + 3] = fTrack;
            }
        }
    }
}

float Level::terrainHeight(float x, float z, float fTrack){
    float mountainBase = 20.f,
            mountainDiff = 60.f,
            valleyBase = -10.f,
            valleyDiff = 20.f,
            landscapeFreq = 0.025f;
    float rockBegin = 20.f,
            rockEnd = 40.f,
            rockFreq = 0.2f,
            rockMinInfluence = 1.f,
            rockMaxInfluence = 10.f;
    float groundTrackInfluence = 0.1f,
            groundTerrainInfluence = 1.f,
            groundRockInfluence = 2.f,
            groundFreq = 1.f;
    float borderBegin = patchHeight() / 2.f,
            borderEnd = patchHeight();

    float center = totalHeight() / 2.f, //fmin(totalWidth(), totalHeight()) / 2.f,
            dXBorder = x - MathUtil::clamp(center, totalWidth() - center, x),
            dZBorder = z - center,  //MathUtil::clamp(center, totalHeight() - center, z),
            fToBorder = MathUtil::smoothstep(borderBegin, borderEnd, sqrt(dXBorder * dXBorder + dZBorder * dZBorder));

    float height = MathUtil::mix(valleyBase, mountainBase, fToBorder)
                    + m_noiseZ.noise(landscapeFreq * x, landscapeFreq * z)
                    * MathUtil::mix(valleyDiff, mountainDiff, fToBorder);

    float fRockyness = MathUtil::smoothstep(rockBegin, rockEnd, height);
    float rockOffset = m_noiseZ.noise(rockFreq * x, rockFreq * z)
                        * MathUtil::mix(rockMinInfluence, rockMaxInfluence, fRockyness);

    float groundOffset = m_noiseZ.noise(groundFreq * x, groundFreq * z)
                * MathUtil::mix(MathUtil::mix(groundTerrainInfluence, groundRockInfluence, fRockyness), groundTrackInfluence, fTrack);

    return MathUtil::mix(height + rockOffset, trackHeight(), fTrack) + groundOffset;
}

int Level::tMapIndex(int s, int t)
{
    return (t * totalVertexCountS() + s) * 4;
}

float Level::tMapEncode(float value)
{
    return value / 200.f + 0.5f;
}

float Level::tMapDecode(float value)
{
    return (value - 0.5f) * 200.f;
}

}//namespace terminus
