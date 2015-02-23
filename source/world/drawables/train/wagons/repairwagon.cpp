#include "repairwagon.h"

#include <QDebug>

#include <world/scene.h>
#include <resources/resourcemanager.h>
#include <resources/geometry.h>
#include <resources/material.h>
#include <resources/program.h>
#include <world/drawables/projectile.h>

namespace terminus
{

RepairWagon::RepairWagon(std::shared_ptr<Scene> scene, Train *train)
: AbstractWagon(scene, train)
{
    m_program = ResourceManager::getInstance()->getProgram("basicShader");
    m_geometry = ResourceManager::getInstance()->getGeometry("repair_repair");
    m_material = ResourceManager::getInstance()->getMaterial("base_Violet");
    initializePhysics(new btBoxShape(btVector3(2.5, 1.0, 1.0)), 1000.f);
}

RepairWagon::~RepairWagon()
{
    deallocatePhysics();
}


void RepairWagon::primaryAction()
{
}

void RepairWagon::localRenderSetup(QOpenGLFunctions& gl, Program & program) const
{
    program.setUniform(std::string("lightDirection"), QVector3D(100.0, 20.0, -100.0));
}

float RepairWagon::length() const
{
    return 7.5f;
}

} //namespace terminus
