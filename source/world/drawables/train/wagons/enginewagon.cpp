#include "enginewagon.h"

#include <QDebug>
#include <QMatrix4x4>
#include <QVector3D>

#include <resources/geometry.h>
#include <resources/material.h>
#include <resources/program.h>
#include <resources/resourcemanager.h>
#include <resources/soundmanager.h>
#include <world/drawables/track.h>
#include <world/drawables/train/train.h>
#include <world/light.h>
#include <world/world.h>

namespace terminus
{

EngineWagon::EngineWagon(World & world, Train * train)
: AbstractWagon(world, train)
{
    setHealth(maxHealth());

    m_program = ResourceManager::getInstance()->getProgram("basicShader");
    if(m_train->track()->isRightTrack())
    {
        m_geometry = ResourceManager::getInstance()->getGeometry("engine_right");
    }
    else
    {
        m_geometry = ResourceManager::getInstance()->getGeometry("engine_left");
    }
    m_material = ResourceManager::getInstance()->getMaterial("base_engineMat");

    QVector3D bb = (maxBB() - minBB()) / 2.f;
    initializePhysics(new btBoxShape(btVector3(bb.x(), bb.y(), bb.z())), 1000.f);

    m_headLight = m_world.lightManager().add(Light::createSpot({1.f, 0.5f, 0.f}, lightPosition(), worldFront(), 64.f, 45.f, 0.4f));
}

EngineWagon::~EngineWagon()
{
    deallocatePhysics();
}

void EngineWagon::primaryActionInternal()
{
    m_train->setVelocity(m_train->velocity() + 0.002f);

    SoundManager::getInstance()->playSound("engineAction");
}

float EngineWagon::cooldownTime() const
{
    return 15000.f;
}

float EngineWagon::maxHealth() const
{
    return 200.f;
}

void EngineWagon::localUpdate()
{
    auto & light = m_world.lightManager().get(m_headLight);
    light.setPosition(lightPosition());
    light.setDirection(worldFront());
    
    std::string materialName = "base_engineMat";
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

    if(!localManager->sound("machine")->isPlaying())
    {
        localManager->playSound("machine");
    }
}

QVector3D EngineWagon::lightPosition()
{
    return modelToWorld({maxBB().x(), maxBB().y() * 0.5f, 0.f});
}

WagonType EngineWagon::wagonType() const
{
    return ENGINE_WAGON;
}

}
