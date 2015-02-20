#include "turret.h"

#include <resources/resourcemanager.h>
#include <world/drawables/abstractgraphicsobject.h>

namespace terminus
{

Turret::Turret(std::shared_ptr<Scene> scene, std::string geometry, std::string material)
 : KinematicPhysicsObject(scene)
{
    m_program = ResourceManager::getInstance()->getProgram("basicShader");
    m_geometry = ResourceManager::getInstance()->getGeometry(geometry);
    m_material = ResourceManager::getInstance()->getMaterial(material);

    initializePhysics(new btSphereShape(1.0), 1.f);
}

Turret::~Turret()
{
}

void Turret::update(int elapsedMilliseconds, QVector3D position, QQuaternion rotation)
{
    setPosition(position);

    KinematicPhysicsObject::update(elapsedMilliseconds);
}

void Turret::preRender(QOpenGLFunctions& gl, Program & program) const
{
    program.setUniform(std::string("lightDirection"), QVector3D(100.0, 20.0, -100.0));
}

} //terminus
