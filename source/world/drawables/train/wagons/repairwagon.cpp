#include "repairwagon.h"

#include <QDebug>

#include <resources/resourcemanager.h>
#include <resources/geometry.h>
#include <resources/material.h>
#include <resources/program.h>

#include <world/drawables/track.h>
#include <world/drawables/train/train.h>
#include <world/world.h>

namespace terminus
{

RepairWagon::RepairWagon(World & world, Train * train)
: AbstractWagon(world, train)
{
    m_program = ResourceManager::getInstance()->getProgram("basicShader");
    if(m_train->track()->isOtherTrackLeft())
    {
        m_geometry = ResourceManager::getInstance()->getGeometry("repair_right");
    }
    else
    {
        m_geometry = ResourceManager::getInstance()->getGeometry("repair_left");
    }
    m_material = ResourceManager::getInstance()->getMaterial("base_Violet");

    initializePhysics(new btBoxShape(btVector3(2.5, 1.0, 1.0)), 1000.f);
}

RepairWagon::~RepairWagon()
{
    deallocatePhysics();
}

void RepairWagon::primaryAction()
{
    if(isDisabled() || isOnCooldown())
    {
        return;
    }

    for(unsigned int i = 0; i < m_train->size(); i++)
    {
        auto wagon = m_train->wagonAt(i);
        if(wagon->isDisabled())
        {
            continue;
        }
        float missingHealth = wagon->maxHealth() - wagon->currentHealth();
        float healing = wagon->currentHealth() + missingHealth / 4.f;
        wagon->setHealth(healing);
    }

    resetCooldown();
}

float RepairWagon::cooldownTime() const
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

WagonType RepairWagon::wagonType() const
{
    return REPAIR_WAGON;
}

} //namespace terminus
