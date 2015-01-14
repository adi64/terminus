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

EngineWagon::EngineWagon(std::shared_ptr<Scene> scene, Train *train)
: AbstractWagon(scene, train)
{
    auto myShape = new btSphereShape(1.0);
    m_bullet_rigidBody->setCollisionShape(myShape);
    m_bullet_collisionShape.reset(myShape);

    m_bullet_rigidBody->setMassProps(1000.0f, btVector3(0.0f, 0.0f, 0.0f));

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
