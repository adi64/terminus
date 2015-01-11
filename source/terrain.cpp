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

Terrain::Terrain(const std::shared_ptr<Scene> &scene)
    : AbstractPhysicsObject(scene)
    , m_playerTrack(std::unique_ptr<Track>(new Track(scene, QVector3D(-15.0, 1.0, -10.0), QVector3D(100.0, 1.0, -10.0))))
    , m_enemyTrack(std::unique_ptr<Track>(new Track(scene, QVector3D(-15.0, 1.0, 10.0), QVector3D(100.0, 1.0, 10.0))))
{   
    m_position = QVector3D(5.0, 0.0, 0.0);
    m_scaling = QVector3D(2.3, 2.3, 2.3);


    auto shape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);

    auto rotationQuaternion = btQuaternion(m_eulerAngles.x(), m_eulerAngles.y(), m_eulerAngles.z(), 1.0);
    auto positionVector = btVector3(m_position.x(), m_position.y(), m_position.z());

    auto motionState = new btDefaultMotionState(btTransform(rotationQuaternion, positionVector));

    auto mass = btScalar(0.0);

    auto inertia = btVector3(0.0, 0.0, 0.0);

    shape->calculateLocalInertia(mass, inertia);

    auto rigidBodyConstructionInfo = btRigidBody::btRigidBodyConstructionInfo(mass, motionState, shape, inertia);

    m_bullet_rigidBody = std::unique_ptr<btRigidBody>(new btRigidBody(rigidBodyConstructionInfo));

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
