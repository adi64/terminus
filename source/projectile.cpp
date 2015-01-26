#include "projectile.h"

#include <QDebug>

#include "scene.h"
#include "resources/resourcemanager.h"
#include "resources/geometry.h"
#include "resources/material.h"
#include "resources/program.h"

namespace terminus
{

Projectile::Projectile(std::shared_ptr<Scene> scene)
: DynamicPhysicsObject(scene)
, m_ageInMilliseconds(0)
{   
    m_program = ResourceManager::getInstance()->getProgram("basicShader");
    m_geometry = ResourceManager::getInstance()->getGeometry("base_Icosahedron");
    m_material = ResourceManager::getInstance()->getMaterial("base_Red");

    initializePhysics(new btSphereShape(1.0), 1.f);
}

Projectile::~Projectile()
{
    deallocatePhysics();
}

void Projectile::update(int elapsedMilliseconds)
{
    DynamicPhysicsObject::update(elapsedMilliseconds);

    m_ageInMilliseconds += elapsedMilliseconds;
    if(m_ageInMilliseconds > maxAgeInMilliseconds())
    {
        // delete node
        auto scene = m_scene.get();
        m_scene->scheduleAction( [this, scene](){scene->deleteNode(this); delete(this);} );
    }
}

void Projectile::preRender(QOpenGLFunctions & gl, Program & program) const
{
    program.setUniform(std::string("lightDirection"), QVector3D(100.0, 20.0, -100.0));
}

unsigned int Projectile::maxAgeInMilliseconds() const
{
    return 50000;
}

} //namespace terminus
