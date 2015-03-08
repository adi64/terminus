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
    setHealth(maxHealth());

    m_program = ResourceManager::getInstance()->getProgram("basicShader");
    m_geometry = ResourceManager::getInstance()->getGeometry("engineWagon_engineWagon");
    m_material = ResourceManager::getInstance()->getMaterial("base_Orange");

    initializePhysics(new btSphereShape(1.0), 1000.f);

    m_headLight = m_world.lightManager().add(Light::createSpot({1.f, 0.5f, 0.f}, lightPosition(), worldFront(), 64.f, 45.f, 0.4f));
}

EngineWagon::~EngineWagon()
{
    deallocatePhysics();
}

void EngineWagon::primaryActionInternal()
{
    m_train->setVelocity(m_train->velocity() + 0.002f);
}

float EngineWagon::cooldownTime() const
{
    return 15000.f;
}

float EngineWagon::maxHealth() const
{
    return 1000.f;
}

void EngineWagon::localUpdate()
{
    auto & light = m_world.lightManager().get(m_headLight);
    light.setPosition(lightPosition());
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

QVector3D EngineWagon::lightPosition()
{
    return modelToWorld({minBB().x(), 0.f, 0.f});
}

float EngineWagon::length() const
{
    return 15.44f;
}

WagonType EngineWagon::wagonType() const
{
    return ENGINE_WAGON;
}

}
