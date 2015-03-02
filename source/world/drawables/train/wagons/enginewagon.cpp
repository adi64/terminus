#include "enginewagon.h"

#include <QDebug>
#include <QMatrix4x4>
#include <QVector3D>

#include <world/world.h>
#include <resources/resourcemanager.h>
#include <resources/soundmanager.h>
#include <resources/geometry.h>
#include <resources/material.h>
#include <resources/program.h>

namespace terminus
{

EngineWagon::EngineWagon(World & world, Train * train)
: AbstractWagon(world, train)
{
    m_program = ResourceManager::getInstance()->getProgram("basicShader");
    m_geometry = ResourceManager::getInstance()->getGeometry("engine_engine");
    m_material = ResourceManager::getInstance()->getMaterial("base_Orange");

    initializePhysics(new btSphereShape(1.0), 1000.f);
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

    m_onCooldown = true;
    m_cooldown = 0.f;
}

float EngineWagon::cooldownRate() const
{
    return 15000.f;
}

void EngineWagon::localUpdate()
{
    std::string materialName = "base_Orange";
    if(isDisabled())
    {
        materialName = "base_Grey";
    }

    m_material = ResourceManager::getInstance()->getMaterial(materialName);

    AbstractWagon::localUpdate();
}

void EngineWagon::localRenderSetup(QOpenGLFunctions& gl, Program & program) const
{
    program.setUniform(std::string("lightDirection"), QVector3D(100.0, 20.0, -100.0));
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
