#include "repairwagon.h"

#include <QDebug>

#include <world/world.h>
#include <resources/resourcemanager.h>
#include <resources/geometry.h>
#include <resources/material.h>
#include <resources/program.h>

namespace terminus
{

RepairWagon::RepairWagon(World & world, Train * train)
: AbstractWagon(world, train)
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
    if(isDisabled() || m_onCooldown)
    {
        return;
    }

    m_onCooldown = true;
    m_cooldown = 0.f;
}

float RepairWagon::cooldownRate() const
{
    return 10000.f;
}

void RepairWagon::localUpdate()
{
    std::string materialName = "base_Violet";
    if(isDisabled())
    {
        materialName = "base_Grey";
    }

    m_material = ResourceManager::getInstance()->getMaterial(materialName);

    AbstractWagon::localUpdate();
}

void RepairWagon::localRenderSetup(QOpenGLFunctions& gl, Program & program) const
{
    program.setUniform(std::string("lightDirection"), QVector3D(100.0, 20.0, -100.0));
}

float RepairWagon::length() const
{
    return 7.5f;
}

WagonType RepairWagon::wagonType() const
{
    return REPAIR_WAGON;
}

} //namespace terminus
