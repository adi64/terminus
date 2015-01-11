#include "enginewagon.h"

#include <QDebug>
#include <QMatrix4x4>
#include <QVector3D>

#include "../scene.h"
#include "../resources/resourcemanager.h"
#include "../resources/soundmanager.h"
#include "../resources/geometry.h"
#include "../resources/material.h"
#include "../resources/program.h"

namespace terminus
{

EngineWagon::EngineWagon(const std::shared_ptr<Scene> &scene, Train *train)
: AbstractWagon(scene, train)
{
    auto shape = new btSphereShape(1.0);

    auto rotationQuaternion = btQuaternion(m_eulerAngles.x(), m_eulerAngles.y(), m_eulerAngles.z(), 1.0);
    auto positionVector = btVector3(m_position.x(), m_position.y(), m_position.z());

    auto motionState = new btDefaultMotionState(btTransform(rotationQuaternion, positionVector));

    auto mass = btScalar(1000.0);

    auto inertia = btVector3(0.0, 0.0, 0.0);

    shape->calculateLocalInertia(mass, inertia);

    auto rigidBodyConstructionInfo = btRigidBody::btRigidBodyConstructionInfo(mass, motionState, shape, inertia);

    m_bullet_rigidBody = std::unique_ptr<btRigidBody>(new btRigidBody(rigidBodyConstructionInfo));

    m_bullet_rigidBody->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);

    m_scene->bullet_world()->addRigidBody(m_bullet_rigidBody.get());
}

void EngineWagon::render(QOpenGLFunctions& gl) const
{
    Program & program = **(ResourceManager::getInstance()->getProgram("basicShader"));
    Material & material = **(ResourceManager::getInstance()->getMaterial("base_Orange"));
    Geometry & geometry = **(ResourceManager::getInstance()->getGeometry("base_Icosahedron"));

    program.bind();

    m_scene->camera().setMatrices(program, m_modelMatrix);
    material.setUniforms(program);
    program.setUniform(std::string("lightDirection"), QVector3D(100.0, 20.0, -100.0));
    geometry.setAttributes(program);

    geometry.draw(gl);

    program.release();

    playSound();    //sounds get played here for the moment
}

void EngineWagon::playSound() const
{
    SoundManager * localManager = SoundManager::getInstance();

    if(!localManager->sound("machine")->isPlaying())        //maybe defining the strings in a typedef?
    {
      localManager->playSoundDistant("machine", calculateDistance());
    }
}

float EngineWagon::length() const
{
    return 2.0f;
}

}
