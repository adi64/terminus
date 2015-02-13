#include "turret.h"

#include <resources/resourcemanager.h>

namespace terminus
{

Turret::Turret(std::shared_ptr<Scene> scene, std::string geometry, std::string material)
 : KinematicPhysicsObject(scene)
{
    m_program = ResourceManager::getInstance()->getProgram("basicShader");
//    m_geometry = ResourceManager::getInstance()->getGeometry(geometry);
//    m_material = ResourceManager::getInstance()->getMaterial(material);
    m_geometry = ResourceManager::getInstance()->getGeometry("weapon_weapon");
    m_material = ResourceManager::getInstance()->getMaterial("base_Blue");
}

Turret::~Turret()
{
}

void Turret::update(int elapsedMilliseconds, QVector3D position, QQuaternion rotation)
{
    m_scale = QVector3D(2, 2, 2);
    setPosition(position);
}

void Turret::preRender(QOpenGLFunctions& gl, Program & program) const
{
    program.setUniform(std::string("lightDirection"), QVector3D(100.0, 20.0, -100.0));
}

} //terminus
