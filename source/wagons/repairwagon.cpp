#include "repairwagon.h"

#include <QDebug>

#include "../scene.h"
#include "../resources/resourcemanager.h"
#include "../resources/geometry.h"
#include "../resources/material.h"
#include "../resources/program.h"
#include "../projectile.h"

namespace terminus
{

RepairWagon::RepairWagon(std::shared_ptr<Scene> scene, Train *train)
: AbstractWagon(scene, train)
{
    auto myShape = new btBoxShape(btVector3(2.5, 1.0, 1.0));
    m_btRigidBody->setCollisionShape(myShape);
    m_btCollisionShape.reset(myShape);

    m_btRigidBody->setMassProps(1000.0f, btVector3(0.0f, 0.0f, 0.0f));

    m_scene->bullet_world()->addRigidBody(m_btRigidBody.get());
}

void RepairWagon::primaryAction()
{
    if(isDisabled())
    {
        return;
    }
}

void RepairWagon::render(QOpenGLFunctions& gl) const
{
    Program & program = **(ResourceManager::getInstance()->getProgram("basicShader"));

    std::string materialName = "base_Violet";
    if(isDisabled())
    {
        materialName = "base_Gray";
    }

    Material & material = **(ResourceManager::getInstance()->getMaterial(materialName));
    Geometry & geometry = **(ResourceManager::getInstance()->getGeometry("repair_repair"));

    program.bind();

    m_scene->camera().setMatrices(program, modelMatrix());
    material.setUniforms(program);
    program.setUniform(std::string("lightDirection"), QVector3D(100.0, 20.0, -100.0));
    geometry.setAttributes(program);

    geometry.draw(gl);

    program.release();
}

float RepairWagon::length() const
{
    return 7.5f;
}

} //namespace terminus
