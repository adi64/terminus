#include "enginewagon.h"

#include <QDebug>
#include <QMatrix4x4>
#include <QVector3D>

#include <resources/resourcemanager.h>
#include <resources/light.h>
#include <resources/soundmanager.h>
#include <resources/geometry.h>
#include <resources/material.h>
#include <resources/program.h>

#include <world/drawables/train/train.h>
#include <world/world.h>

namespace terminus
{

EngineWagon::EngineWagon(World & world, Train * train)
: AbstractWagon(world, train)
{
    m_program = ResourceManager::getInstance()->getProgram("basicShader");
    m_geometry = ResourceManager::getInstance()->getGeometry("engine_engine");
    m_material = ResourceManager::getInstance()->getMaterial("base_Orange");

    initializePhysics(new btSphereShape(1.0), 1000.f);

    m_headLight = m_world.lightManager().add(Light::createSpot({1.f, 0.5f, 0.f}, position(), worldFront(), 64.f, 45.f, 0.4f));
}

EngineWagon::~EngineWagon()
{
    deallocatePhysics();
}

void EngineWagon::primaryAction()
{
    if(isDisabled() || m_onCooldown)
    {
        return;
    }

    m_train->setVelocity(m_train->velocity() + 0.002f);

    m_onCooldown = true;
    m_cooldown = 0.f;
}

float EngineWagon::cooldownRate() const
{
    return 15000.f;
}

void EngineWagon::localUpdate()
{
    auto & light = m_world.lightManager().get(m_headLight);
    light.setPosition(position());
    light.setDirection(worldFront());
    
    std::string materialName = "base_Orange";
    if(isDisabled())
    {
        materialName = "base_Grey";
    }

    m_material = ResourceManager::getInstance()->getMaterial(materialName);

    AbstractWagon::localUpdate();
}

void EngineWagon::playSound() const
{
    SoundManager * localManager = SoundManager::getInstance();

    if(!localManager->sound("machine")->isPlaying())        //maybe defining the strings in a typedef?
    {
        localManager->playSound("machine");
    }
}

float EngineWagon::length() const
{
    return 6.0f;
}

WagonType EngineWagon::wagonType() const
{
    return ENGINE_WAGON;
}

}
