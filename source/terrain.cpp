#include "terrain.h"

#include <QDebug>

#include "track.h"
#include "scene.h"
#include "resources/resourcemanager.h"
#include "resources/geometry.h"
#include "resources/material.h"
#include "resources/program.h"

namespace terminus
{

Terrain::Terrain(std::shared_ptr<Scene> scene)
    : KinematicPhysicsObject(scene)
    , m_playerTrack(std::unique_ptr<Track>(new Track(scene, QVector3D(-15.0, 1.0, -10.0), QVector3D(100.0, 1.0, -10.0))))
    , m_enemyTrack(std::unique_ptr<Track>(new Track(scene, QVector3D(-15.0, 1.0, 10.0), QVector3D(100.0, 1.0, 10.0))))
{   
    setPosition(QVector3D(5.0f, 0.0f, 0.0f));
    m_scaling = QVector3D(2.3f, 2.3f, 2.3f);

    // infinite plane
    auto myShape = new btStaticPlaneShape(btVector3(0.0f, 1.0f, 0.0f), 1.0f);
    m_bullet_rigidBody->setCollisionShape(myShape);
    m_bullet_collisionShape.reset(myShape);

    // zero mass --> unlimited mass, does not move
    m_bullet_rigidBody->setMassProps(0.0f, btVector3(0.0f, 0.0f, 0.0f));

    m_scene->bullet_world()->addRigidBody(m_bullet_rigidBody.get());
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
    AbstractPhysicsObject::update(elapsedMilliseconds);

    // update tracks
    m_playerTrack->update(elapsedMilliseconds);
    m_enemyTrack->update(elapsedMilliseconds);
}

void Terrain::render(QOpenGLFunctions& gl) const
{
    // render terrain
    Program & program = **(ResourceManager::getInstance()->getProgram("basicShader"));
    Material & material = **(ResourceManager::getInstance()->getMaterial("base_Green"));
    Geometry & geometry = **(ResourceManager::getInstance()->getGeometry("terrain_Terrain"));

    program.bind();

    m_scene->camera().setMatrices(program, m_modelMatrix);
    material.setUniforms(program);
    program.setUniform(std::string("lightDirection"), QVector3D(100.0, 20.0, -100.0));
    geometry.setAttributes(program);

    geometry.draw(gl);

    program.release();

    // render tracks
    m_playerTrack->render(gl);
    m_enemyTrack->render(gl);
}

} //namespace terminus
