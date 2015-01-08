#include "projectile.h"

#include <QDebug>

#include <bullet/btBulletDynamicsCommon.h>

#include "scene.h"
#include "resources/resourcemanager.h"
#include "resources/geometry.h"
#include "resources/material.h"
#include "resources/program.h"

namespace terminus
{

Projectile::Projectile(Scene *scene)
    : AbstractGraphicsObject(scene)
{   
    auto shape = new btSphereShape(1.0);

    auto rotationQuaternion = btQuaternion(eulerAngles().x(), eulerAngles().y(), eulerAngles().z(), 1.0);
    auto positionVector = btVector3(position().x(), position().y(), position().z());

    auto motionState = new btDefaultMotionState(btTransform(rotationQuaternion, positionVector));

    auto mass = btScalar(1.0);

    auto inertia = btVector3(0.0, 0.0, 0.0);

    shape->calculateLocalInertia(mass, inertia);

    auto rigidBodyConstructionInfo = btRigidBody::btRigidBodyConstructionInfo(mass, motionState, shape, inertia);

    m_bullet_rigidBody = std::unique_ptr<btRigidBody>(new btRigidBody(rigidBodyConstructionInfo));

    m_scene->bullet_world()->addRigidBody(m_bullet_rigidBody.get());
}

void Projectile::setPosition(const QVector3D &newPosition)
{
    AbstractGraphicsObject::setPosition(newPosition);

    /*
    btTransform worldTransform;

    static_cast<btRigidBody*>(m_bullet_rigidBody.get())->getMotionState()->getWorldTransform(worldTransform);

    worldTransform.setOrigin(btVector3(newPosition.x(), newPosition.y(), newPosition.z()));

    static_cast<btRigidBody*>(m_bullet_rigidBody.get())->getMotionState()->setWorldTransform(worldTransform);



    qDebug() << "TODO: Set new Position of projectile";
    */
}

void Projectile::applyForce(const QVector3D &force)
{
    static_cast<btRigidBody*>(m_bullet_rigidBody.get())->applyCentralForce(btVector3(force.x(), force.y(), force.z()));
}

void Projectile::update(int elapsedMilliseconds)
{
    AbstractGraphicsObject::update(elapsedMilliseconds);


    btTransform transform;
    m_bullet_rigidBody->getMotionState()->getWorldTransform(transform);

    auto rigidBodyPosition = transform.getOrigin();

    setPosition(QVector3D(rigidBodyPosition.getX(), rigidBodyPosition.getY(), rigidBodyPosition.getZ()));

    m_ageInMilliseconds += elapsedMilliseconds;
    if(m_ageInMilliseconds > maxAgeInMilliseconds())
    {
        // please delete
    }
}


void Projectile::render(QOpenGLFunctions& gl) const
{
    // render terrain
    Program & program = **(ResourceManager::getInstance()->getProgram("basicShader"));
    Material & material = **(ResourceManager::getInstance()->getMaterial("base_Red"));
    Geometry & geometry = **(ResourceManager::getInstance()->getGeometry("base_Icosahedron"));

    program.bind();

    m_scene->camera().setMatrices(program, m_modelMatrix);
    material.setUniforms(program);
    program.setUniform(std::string("lightDirection"), QVector3D(100.0, 20.0, -100.0));
    geometry.setAttributes(program);

    geometry.draw(gl);

    program.release();
}

unsigned int Projectile::maxAgeInMilliseconds() const
{
    return 5000;
}

} //namespace terminus
