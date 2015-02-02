#include "terrain.h"

#include <QPoint>
#include <QDebug>

#include <string>

#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

#include "track.h"
#include "scene.h"
#include "resources/resourcemanager.h"
#include "resources/geometry.h"
#include "resources/material.h"
#include "resources/program.h"

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

Terrain::Terrain(std::shared_ptr<Scene> scene)
: KinematicPhysicsObject(scene)
, m_terrainMapOnGPU(false)
{   
    m_program = ResourceManager::getInstance()->getProgram("terrain");
    m_geometry = ResourceManager::getInstance()->getGeometry("terrain_patch");
    m_material = ResourceManager::getInstance()->getMaterial("base_Terrain");

    m_level.generateLevel();
    setScale(m_level.scale());
    m_playerTrack = std::unique_ptr<Track>(new Track(scene, m_level.playerTrack()));
    m_enemyTrack = std::unique_ptr<Track>(new Track(scene, m_level.enemyTrack()));
    
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

Track *Terrain::playerTrack() const
{
    return m_playerTrack.get();
}

Track *Terrain::enemyTrack() const
{
    return m_enemyTrack.get();
}

void Terrain::update(int elapsedMilliseconds)
{
    QVector3D pos = position();
    QQuaternion rot = rotation();

    btTransform transform = m_btRigidBody->getCenterOfMassTransform();
    transform.setOrigin(btVector3(pos.x() + m_level.totalWidth() / 2.f, pos.y(), pos.z() + m_level.totalHeight() / 2.f));
    transform.setRotation(btQuaternion(rot.x(), rot.y(), rot.z(), rot.scalar()));

    AbstractPhysicsObject::update(elapsedMilliseconds);

    m_playerTrack->update(elapsedMilliseconds);
    m_enemyTrack->update(elapsedMilliseconds);
}

void Terrain::render(QOpenGLFunctions& gl) const
{
    QVector3D camPos = m_scene->camera().eye();
    QPoint pid = m_level.positionToPatchID(camPos.x(), camPos.z());

    int radius = 5;
    for(int iX = std::max(0, pid.x() - radius); iX < std::min(m_level.patchCountS(), pid.x() + radius); iX++)
    {
        for(int iZ = std::max(0, pid.y() - radius); iZ < std::min(m_level.patchCountT(), pid.y() + radius); iZ++)
        {
            m_currentPatchX = iX;
            m_currentPatchZ = iZ;
            AbstractGraphicsObject::render(gl);
        }
    }

    // render tracks
    m_playerTrack->render(gl);
    m_enemyTrack->render(gl);
}

void Terrain::preRender(QOpenGLFunctions & gl, Program & program) const
{
    program.setUniform("lightDirection", QVector3D(100.0, 20.0, -100.0));
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

void Terrain::postRender(QOpenGLFunctions & gl, Program & program) const
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

} //namespace terminus
