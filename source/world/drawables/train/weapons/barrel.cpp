#include "barrel.h"

#include <resources/resourcemanager.h>

namespace terminus
{

Barrel::Barrel(std::shared_ptr<Scene> scene, std::string geometry, std::string material)
 : KinematicPhysicsObject(scene)
{
    m_program = ResourceManager::getInstance()->getProgram("basicShader");
    m_geometry = ResourceManager::getInstance()->getGeometry(geometry);
    m_material = ResourceManager::getInstance()->getMaterial(material);

    initializePhysics(new btSphereShape(1.0), 1.f);
}

Barrel::~Barrel()
{
}

void Barrel::update(int elapsedMilliseconds, QVector3D position, QQuaternion rotation)
{
    setPosition(position);

    KinematicPhysicsObject::update(elapsedMilliseconds);
}

} //terminus
