#include "terrain.h"

#include <QPoint>
#include <QDebug>

#include <string>

#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

#include <player/localplayer.h>
#include <resources/resourcemanager.h>
#include <resources/geometry.h>
#include <resources/material.h>
#include <resources/program.h>
#include <world/drawables/track.h>
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

Terrain::Terrain(World & world)
: KinematicPhysicsObject(world)
, m_terrainMapOnGPU(false)
{   
    m_program = ResourceManager::getInstance()->getProgram("terrain");
    m_geometry = ResourceManager::getInstance()->getGeometry("terrain_patch");
    m_material = ResourceManager::getInstance()->getMaterial("base_Terrain");

    m_level.generateLevel();
    setScale(m_level.scale());
    m_rightTrack = std::unique_ptr<Track>(new Track(m_world, m_level.rightTrack(), true));
    m_leftTrack = std::unique_ptr<Track>(new Track(m_world, m_level.enemyTrack(), false));
    
    auto shape = new btHeightfieldTerrainShape(m_level.heightMapSizeS(),
                                               m_level.heightMapSizeT(),
                                               m_level.heightMapData(),
                                               1.f /*ignored for PHY_FLOAT*/,
                                               0.f, 200.f,
                                               1 /*y axix*/,
                                               PHY_ScalarType::PHY_FLOAT,
                                               false);

    shape->setLocalScaling(btVector3(m_level.heightMapScaleS(), 1.f, m_level.heightMapScaleT()));
    initializePhysics(shape, 1.f);

    setPosition(QVector3D(0.f, 0.f, 0.f)); //TODO centered collision object
}

Terrain::~Terrain()
{
    deallocatePhysics();
}

Track *Terrain::rightTrack() const
{
    return m_rightTrack.get();
}

Track *Terrain::leftTrack() const
{
    return m_leftTrack.get();
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

void Terrain::localUpdate()
{
    QVector3D pos = position();
    QQuaternion rot = rotation();

    btTransform transform = m_btRigidBody->getCenterOfMassTransform();
    transform.setOrigin(btVector3(pos.x() + m_level.totalWidth() / 2.f, pos.y(), pos.z() + m_level.totalHeight() / 2.f));
    transform.setRotation(btQuaternion(rot.x(), rot.y(), rot.z(), rot.scalar()));

    AbstractPhysicsObject::localUpdate();
}

void Terrain::localRender(QOpenGLFunctions& gl) const
{
    QVector3D camPos = m_world.localPlayer().camera().eye();
    QPoint patchID = m_level.positionToPatchID(camPos.x(), camPos.z());

    const int radius = 3;
    for(int iX = std::max(0, patchID.x() - radius); iX < std::min(m_level.patchCountS(), patchID.x() + radius); iX++)
    {
        for(int iZ = std::max(0, patchID.y() - radius); iZ < std::min(m_level.patchCountT(), patchID.y() + radius); iZ++)
        {
            m_currentPatchX = iX;
            m_currentPatchZ = iZ;
            AbstractGraphicsObject::localRender(gl);
        }
    }

    // render tracks
    m_rightTrack->render(gl);
    m_leftTrack->render(gl);
}

void Terrain::localRenderSetup(QOpenGLFunctions & gl, Program & program) const
{
    AbstractGraphicsObject::localRenderSetup(gl, program);

    program.setUniform("levelMap", 0);
    QVector4D texInfo(static_cast<float>(m_currentPatchX * (m_level.vertexCountS() - 1)),
                      static_cast<float>(m_currentPatchZ * (m_level.vertexCountT() - 1)),
                      static_cast<float>(m_level.totalVertexCountS() - 1),
                      static_cast<float>(m_level.totalVertexCountT() - 1));
    QVector4D posInfo(m_level.vertexWidthUnscaled(),
                      m_level.vertexHeightUnscaled(),
                      m_currentPatchX * m_level.patchWidthUnscaled(),
                      m_currentPatchZ * m_level.patchHeightUnscaled());
    program.setUniform("texInfo", texInfo);
    program.setUniform("posInfo", posInfo);

    allocateTerrainMap(gl);
    gl.glActiveTexture(GL_TEXTURE0);
    gl.glBindTexture(GL_TEXTURE_2D, m_terrainMap);
}

void Terrain::localRenderCleanup(QOpenGLFunctions & gl, Program & /*program*/) const
{
    gl.glActiveTexture(GL_TEXTURE0);
    gl.glBindTexture(GL_TEXTURE_2D, 0);
}

void Terrain::allocateTerrainMap(QOpenGLFunctions & gl) const
{
    if(m_terrainMapOnGPU)
        return;

    const void * data = m_level.terrainMapData();

    gl.glGenTextures(1, &m_terrainMap);
    gl.glBindTexture(GL_TEXTURE_2D, m_terrainMap);
    gl.glTexImage2D(GL_TEXTURE_2D, 0, TEXFORMAT, m_level.totalVertexCountS(), m_level.totalVertexCountT(), 0, GL_RGBA, GL_FLOAT, data);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    gl.glBindTexture(GL_TEXTURE_2D, 0);

    m_terrainMapOnGPU = true;
}

void Terrain::deallocateTerrainMap(QOpenGLFunctions & gl) const
{
    if(!m_terrainMapOnGPU)
        return;

    gl.glDeleteTextures(1, &m_terrainMap);

    m_terrainMapOnGPU = false;
}

short Terrain::myCollisionType() const
{
    return BulletWorld::CollisionTypes::COLLISIONTYPE_TERRAIN;
}

short Terrain::possibleCollisionTypes() const
{
    // collision detection with terrain does not work yet
    return BulletWorld::CollisionTypes::COLLISIONTYPE_NOTHING;
}

} //namespace terminus
