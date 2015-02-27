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

    m_headLight = m_world.lightManager().addSpotLight(position(), worldFront(), QVector3D(), {1.f, 0.5f, 0.f});
}

EngineWagon::~EngineWagon()
{
    deallocatePhysics();
}

void EngineWagon::localUpdate(int elapsedMilliseconds)
{
    auto & light = m_world.lightManager().light(m_headLight);
    light.position = position();
    light.direction = worldFront();

    AbstractWagon::localUpdate(elapsedMilliseconds);
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

}
