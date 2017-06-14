#include "terrain.h"

#include <cassert>
#include <string>

#include <glincl.h>

#include <QPoint>

#include <player/localplayer.h>
#include <resources/resourcemanager.h>
#include <world/camera.h>
#include <world/level.h>
#include <world/world.h>

#define TEXFORMAT GL_RGBA
#ifdef Q_OS_LINUX
    #ifndef Q_OS_ANDROID
        #undef TEXFORMAT
        #define TEXFORMAT GL_RGBA16F
    #endif
#endif
#ifdef Q_OS_WIN32
    #ifndef Q_OS_WINPHONE
        #undef TEXFORMAT
        #define TEXFORMAT GL_RGBA16F
    #endif
#endif
#ifdef Q_OS_MAC
    #ifndef Q_OS_IOS
        #undef TEXFORMAT
        #define TEXFORMAT GL_RGBA16F
    #endif
#endif

namespace terminus
{

Terrain::Terrain(World & world, const Level & level)
: AbstractGraphicsObject(world)
, m_terrainMapOnGPU(false)
{
    m_program = ResourceManager::getInstance()->getProgram("terrain");
    m_geometry = ResourceManager::getInstance()->getGeometry("terrain_patch");
    m_material = ResourceManager::getInstance()->getMaterial("base_Terrain");

    m_rightTrack = std::unique_ptr<Track>(new Track(m_world, true));
    m_leftTrack = std::unique_ptr<Track>(new Track(m_world, false));

    configureWith(level);

    setPosition(QVector3D(0.f, 0.f, 0.f)); //TODO centered collision object
}

Track & Terrain::rightTrack() const
{
    assert(m_rightTrack);
    return *(m_rightTrack.get());
}

Track & Terrain::leftTrack() const
{
    assert(m_leftTrack);
    return *(m_leftTrack.get());
}

void Terrain::doForAllChildren(std::function<void (AbstractGraphicsObject &)> callback)
{
    if(m_leftTrack)
    {
        callback(*m_leftTrack);
    }
    if(m_rightTrack)
    {
        callback(*m_rightTrack);
    }
}

void Terrain::localRender() const
{
    QVector3D camPos = m_world.localPlayer().camera().eye();
    QPoint patchID = m_levelConfig.positionToPatchID(camPos.x(), camPos.z());

    const int radius = 3;
    for(int iX = std::max(0, patchID.x() - radius); iX < std::min(m_levelConfig.patchCountS(), patchID.x() + radius); iX++)
    {
        for(int iZ = std::max(0, patchID.y() - radius); iZ < std::min(m_levelConfig.patchCountT(), patchID.y() + radius); iZ++)
        {
            m_currentPatchX = iX;
            m_currentPatchZ = iZ;
            AbstractGraphicsObject::localRender();
        }
    }

    // render tracks
    m_rightTrack->render();
    m_leftTrack->render();
}

void Terrain::localRenderSetup(Program & program) const
{
    AbstractGraphicsObject::localRenderSetup(program);

    program.setUniform("levelMap", 0);
    QVector4D texInfo(static_cast<float>(m_currentPatchX * (m_levelConfig.vertexCountS() - 1)),
                      static_cast<float>(m_currentPatchZ * (m_levelConfig.vertexCountT() - 1)),
                      static_cast<float>(m_levelConfig.totalVertexCountS() - 1),
                      static_cast<float>(m_levelConfig.totalVertexCountT() - 1));
    QVector4D posInfo(m_levelConfig.vertexWidthUnscaled(),
                      m_levelConfig.vertexHeightUnscaled(),
                      m_currentPatchX * m_levelConfig.patchWidthUnscaled(),
                      m_currentPatchZ * m_levelConfig.patchHeightUnscaled());
    program.setUniform("texInfo", texInfo);
    program.setUniform("posInfo", posInfo);

    allocateTerrainMap();
    gl.glActiveTexture(GL_TEXTURE0);
    gl.glBindTexture(GL_TEXTURE_2D, m_terrainMap);
}

void Terrain::localRenderCleanup(Program & /*program*/) const
{
    gl.glActiveTexture(GL_TEXTURE0);
    gl.glBindTexture(GL_TEXTURE_2D, 0);
}

void Terrain::configureWith(const Level & level)
{
    m_levelConfig = level.config();
    setScale(m_levelConfig.scale());
    m_leftTrack->setCourse(level.createLeftTrackCourse());
    m_rightTrack->setCourse(level.createRightTrackCourse());
    m_terrainMapData.reset(level.copyTerrainMapData());
    m_terrainMapValid = true;
}

void Terrain::allocateTerrainMap() const
{
    if(m_terrainMapOnGPU && m_terrainMapValid)
        return;

    if(!m_terrainMapOnGPU)
        gl.glGenTextures(1, &m_terrainMap);

    gl.glBindTexture(GL_TEXTURE_2D, m_terrainMap);
    glTexImage2D(GL_TEXTURE_2D, 0, TEXFORMAT, m_levelConfig.totalVertexCountS(), m_levelConfig.totalVertexCountT(), 0, GL_RGBA, GL_FLOAT, m_terrainMapData->data());
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    gl.glBindTexture(GL_TEXTURE_2D, 0);

    m_terrainMapOnGPU = true;
}

void Terrain::deallocateTerrainMap() const
{
    if(!m_terrainMapOnGPU)
        return;

    gl.glDeleteTextures(1, &m_terrainMap);

    m_terrainMapOnGPU = false;
}

} //namespace terminus
