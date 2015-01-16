#include "level.h"

#include <QVector2D>
#include <QVector4D>
#include <QDebug>

#include "mathutil.h"
#include "perlinnoise.h"

namespace terminus
{

Level::Level()
: m_vertexCountS(64)
, m_vertexCountT(73)
, m_patchCountS(15)
, m_patchCountT(3)
, m_vertexWidth(1.f)
, m_vertexHeight(sqrt(3.f)/2.f)
, m_scale(4.f)
, m_noiseX(/*seed*/)
, m_noiseY(/*seed*/)
, m_noiseZ(/*seed*/)
, m_tracksGenerated(false)
, m_trackHeight(50.f)
, m_texGenerated(false)
, m_terrainMapData(totalVertexCountS() * totalVertexCountT() * 4, 0.f)
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

int Level::vertexCountS() const
{
    return m_vertexCountS;
}
int Level::vertexCountT() const
{
    return m_vertexCountT;
}
int Level::patchCountS() const
{
    return m_patchCountS;
}
int Level::patchCountT() const
{
    return m_patchCountT;
}
int Level::totalVertexCountS() const
{
    return m_patchCountS * (m_vertexCountS - 1) + 1;
}
int Level::totalVertexCountT() const
{
    return m_patchCountT * (m_vertexCountT - 1) + 1;
}
float Level::vertexWidthUnscaled() const
{
    return m_vertexWidth;
}
float Level::vertexHeightUnscaled() const
{
    return m_vertexHeight;
}
float Level::patchWidthUnscaled() const
{
    return (vertexCountS() - 1) * vertexWidthUnscaled();
}
float Level::patchHeightUnscaled() const
{
    return (vertexCountT() - 1) * vertexHeightUnscaled();
}
float Level::vertexWidth() const
{
    return m_vertexWidth * scale();
}
float Level::vertexHeight() const
{
    return m_vertexHeight * scale();
}
float Level::patchWidth() const
{
    return (vertexCountS() - 1) * vertexWidth();
}
float Level::patchHeight() const
{
    return (vertexCountT() - 1) * vertexHeight();
}
int Level::totalWidth() const
{
    return totalVertexCountS() * vertexWidth();
}
int Level::totalHeight() const
{
    return totalVertexCountT() * vertexHeight();
}
float Level::scale() const
{
    return m_scale;
}
QPoint Level::positionToVertexID(float x, float z) const
{
    int t = round(z / vertexHeight());
    int s = round( x / vertexWidth() - (t % 2 == 0 ? 0.f : 0.5f));
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
    float x = (static_cast<float>(s) + (t % 2 == 0 ? 0.f : 0.5f)) * vertexWidth();//TODO find out why that works
    float z = static_cast<float>(t) * vertexHeight();
    return QVector2D(x, z);
}

const void * Level::terrainMapData() const
{
    //generateTerrainMap(); TODO mechanism to assure previous generateLevel() call

    return m_terrainMapData.data();
}

std::unique_ptr<Polyline> Level::playerTrack() const
{
    std::vector<QVector3D> points;
    if(m_playerTrack)
    {
        for(float param = 0.0; param <= m_playerTrack->length(); param += 0.1)
        {
            QVector2D pointInPlane = m_playerTrack->getPosition(param);
            points.push_back(QVector3D(pointInPlane.x(), trackHeight(), pointInPlane.y()));
        }
    }
    else
    {
        points.push_back(QVector3D(0.f, 0.f, totalHeight()));
        points.push_back(QVector3D(totalWidth(), 0.f, totalHeight()));
    }
    return std::unique_ptr<Polyline>(new Polyline(points));
}

std::unique_ptr<Polyline> Level::enemyTrack() const
{
    std::vector<QVector3D> points;
    if(m_enemyTrack)
    {
        for(float param = 0.0; param <= m_enemyTrack->length(); param += 0.1)
        {
            QVector2D pointInPlane = m_enemyTrack->getPosition(param);
            points.push_back(QVector3D(pointInPlane.x(), trackHeight(), pointInPlane.y()));
        }
    }
    else
    {
        points.push_back(QVector3D(0.f, 0.f, 0.f));
        points.push_back(QVector3D(totalWidth(), 0.f, 0.f));
    }
    return std::unique_ptr<Polyline>(new Polyline(points));
}

float Level::trackHeight() const
{
    return m_trackHeight;
}

void Level::generateTracks(){
    if(m_tracksGenerated)
        return;

    float minDist = 32.f,
            playerMinZ = totalHeight() / 2.f - 20.f,
            playerMaxZ = totalHeight() / 2.f + 100.f,
            enemyMinZ = totalHeight() / 2.f - 100.f,
            enemyMaxZ = totalHeight() / 2.f + 20.f;
    float xStep = 64.f,
            xBegin = totalHeight() / 2.f - xStep,
            xEnd = totalWidth() - totalHeight() / 2.f + xStep;

    std::vector<QVector2D> playerPoints;
    std::vector<QVector2D> enemyPoints;
    for(float x = xBegin; x < xEnd; x += xStep)
    {
        float zPlayer = MathUtil::mix(playerMinZ, playerMaxZ, m_noiseX.asymmetricRnd());
        float zEnemy = MathUtil::mix(enemyMinZ, enemyMaxZ, m_noiseX.asymmetricRnd());
        float distanceCorrect = - fmin(zPlayer - zEnemy - minDist, 0);
        playerPoints.push_back(QVector2D(x, zPlayer + distanceCorrect / 2.f));
        enemyPoints.push_back(QVector2D(x, zEnemy - distanceCorrect / 2.f));
    }

    m_playerTrack = std::unique_ptr<CatmullRomSpline>(new CatmullRomSpline(playerPoints));
    m_enemyTrack = std::unique_ptr<CatmullRomSpline>(new CatmullRomSpline(enemyPoints));

    m_tracksGenerated = true;
}

void Level::generateTerrainMap(){
    if(m_texGenerated)
        return;
    generateTracks();

    for(int iT = 0; iT < totalVertexCountT(); iT++)
    {
        for(int iS = 0; iS < totalVertexCountS(); iS++)
        {
            QVector2D point = vertexIDToPosition(iS, iT);
            int i = tMapIndex(iS, iT);
            QVector2D d = terrainDisplacement(point.x(), point.y());
            tMapSetXYZ(i, d.x(), 0.f, d.y());
            tMapSetW(i, 0.f);
        }
    }

    setTrackEnvironment(*m_playerTrack);
    setTrackEnvironment(*m_enemyTrack);

    for(int iT = 0; iT < totalVertexCountT(); iT++)
    {
        for(int iS = 0; iS < totalVertexCountS(); iS++)
        {
            QVector2D point = vertexIDToPosition(iS, iT);
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
            influence = 0.4 * vertexWidth(),
            dX = m_noiseX.noise(x * scale, z * scale) * influence,
            dZ = m_noiseZ.noise(x * scale, z * scale) * influence;
    return QVector2D(dX, dZ);
}

void Level::setTrackEnvironment(const CatmullRomSpline & track)
{
    float radiusBegin = 1.f,
            radiusEnd = 32.f;
    for(float param = 0.0; param <= track.length(); param += 0.025 * scale())
    {
        QVector2D trackPoint = track.getPosition(param);
        QPoint vid = positionToVertexID(trackPoint.x(), trackPoint.y());
        int radius = ceil(radiusEnd / scale());
        for(int iT = std::max(vid.y() - radius, 0); iT <= std::min(vid.y() + radius, totalVertexCountT()); iT++)
        {
            for(int iS = std::max(vid.x() - radius, 0); iS <= std::min(vid.x() + radius, totalVertexCountS()); iS++)
            {
                QVector2D point = vertexIDToPosition(iS, iT);
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
    float mountainBase = 100.f,
            mountainDiff = 150.f,
            valleyBase = 10.f,
            valleyDiff = 30.f,
            landscapeFreq = 0.01f;
    float rockBegin = 10.f,
            rockEnd = 150.f,
            rockFreq = 0.05f,
            rockMinInfluence = 5.f,
            rockMaxInfluence = 20.f;
    float groundTrackInfluence = 0.1f,
            groundTerrainInfluence = 2.f,
            groundRockInfluence = 5.f,
            groundFreq = 1.f;
    float borderBegin = 25.f,
            borderEnd = 150.f;

    float center = totalHeight() / 2.f, //fmin(totalWidth(), totalHeight()) / 2.f,
            dXBorder = x - MathUtil::clamp(center, totalWidth() - center, x),
            dZBorder = z - center,  //MathUtil::clamp(center, totalHeight() - center, z),
            fToBorder = MathUtil::linstep(borderBegin, borderEnd, sqrt(dXBorder * dXBorder + dZBorder * dZBorder));

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

int Level::tMapIndex(int s, int t) const
{
    return (t * totalVertexCountS() + s) * 4;
}
void Level::tMapSetXYZ(int i, float dx, float dy, float dz)
{
    m_terrainMapData[i] = dx / scale() + 0.5f;
    m_terrainMapData[i + 1] = dy / scale() / 200.f;
    m_terrainMapData[i + 2] = dz / scale() + 0.5f;
}
QVector3D Level::tMapGetXYZ(int i) const
{
    return QVector3D( (m_terrainMapData[i] - 0.5f) * scale(),
                        m_terrainMapData[i + 1] * 200.f * scale(),
                       (m_terrainMapData[i + 2] - 0.5f) * scale());
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
