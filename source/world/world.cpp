#include "world.h"

#include <math.h>

#include <QDebug>
#include <QOpenGLShaderProgram>
#include <QTime>

#include <game.h>
#include <world/drawables/train/train.h>
#include <world/drawables/terrain.h>
#include <world/drawables/skybox.h>
#include <world/drawables/projectile.h>
#include <world/drawables/train/wagons/enginewagon.h>
#include <world/drawables/train/wagons/weaponwagon.h>
#include <world/drawables/train/wagons/repairwagon.h>

#include <player/abstractplayer.h>
#include <player/aiplayer.h>
#include <player/localplayer.h>

#include <world/camera.h>
#include <world/physics/abstractphysicsobject.h>
#include <world/physics/bulletworld.h>
#include <deferredactionhandler.h>

namespace terminus
{

World::World(Game & game)
: m_game(game)
, m_bulletWorld(std::shared_ptr<BulletWorld>(new BulletWorld))
, m_skybox(std::unique_ptr<SkyBox>(new SkyBox(*this)))
, m_terrain(std::unique_ptr<Terrain>(new Terrain(*this)))
, m_rightTrain(std::unique_ptr<Train>(new Train(*this, m_terrain->rightTrack())))
, m_leftTrain(std::unique_ptr<Train>(new Train(*this, m_terrain->leftTrack())))
{
    m_rightTrain->addWagon<WeaponWagon>();
    m_rightTrain->addWagon<WeaponWagon>();
    m_rightTrain->addWagon<RepairWagon>();
    m_rightTrain->addWagon<WeaponWagon>();
    m_rightTrain->addWagon<WeaponWagon>();
    m_rightTrain->addWagon<RepairWagon>();
    m_rightTrain->addWagon<WeaponWagon>();

    m_leftTrain->addWagon<WeaponWagon>();
    m_leftTrain->addWagon<WeaponWagon>();
    m_leftTrain->addWagon<RepairWagon>();
    m_leftTrain->addWagon<WeaponWagon>();
    m_leftTrain->addWagon<WeaponWagon>();
    m_leftTrain->addWagon<WeaponWagon>();
    m_leftTrain->addWagon<RepairWagon>();

    m_leftTrain->follow(m_rightTrain.get());

    m_localPlayer = std::unique_ptr<LocalPlayer>(new LocalPlayer(*this, m_rightTrain.get()));
    m_aiPlayer = std::unique_ptr<AIPlayer>(new AIPlayer(*this, m_leftTrain.get(), m_rightTrain.get()));

    localPlayer().camera().setEye(QVector3D(-30.0, 10.0, 20.0));
    localPlayer().camera().setCenter(QVector3D(0.0, 0.0, 10.0));
    localPlayer().camera().setUp(QVector3D(0.0, 1.0, 0.0));

    m_lightManager.add(Light::createAmbient({0.1f, 0.1f, 0.1f}));
    m_lightManager.add(Light::createDirectional({0.5f, 0.47f, 0.43f}, {-5.0, -1.0, 5.0}));
    m_lightManager.add(Light::createDirectional({0.4f, 0.43f, 0.5f}, {0.0, -1.0, 0.0}));
}

World::~World()
{

}

void World::addObject(AbstractGraphicsObject * node)
{
    m_dynamicObjects.push_back(std::unique_ptr<AbstractGraphicsObject>(node));
}

void World::deleteObject(AbstractGraphicsObject * object)
{
    m_dynamicObjects.remove_if(
        [object](const std::unique_ptr<AbstractGraphicsObject> & candidate)
        {
            return candidate.get() == object;
        });
}

void World::update()
{
    // physics - never give bullet negative step times
    m_bulletWorld->stepSimulation(fmax(m_game.timer().get("frameTimer") / 1000.f, 0.f), 10);

    m_skybox->update();
    m_terrain->update();
    m_rightTrain->update();
    m_leftTrain->update();
    for(auto & object : m_dynamicObjects)
    {
        object->update();
    }

    m_aiPlayer->update();
    m_localPlayer->update();
}

void World::render(QOpenGLFunctions & gl) const
{
    gl.glViewport(0, 0, m_localPlayer->camera().viewport().x(), m_localPlayer->camera().viewport().y());

    gl.glClearColor(0.5f, 0.55f, 0.6f, 1.0f);
    gl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gl.glEnable(GL_BLEND);
    gl.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    gl.glEnable(GL_CULL_FACE);
    gl.glFrontFace(GL_CCW); //TODO check if still necessary after geometry loader

    gl.glEnable(GL_DEPTH_TEST);
    gl.glDepthMask(GL_TRUE);
    gl.glDepthFunc(GL_LESS);


    m_skybox->render(gl);
    m_terrain->render(gl);
    m_rightTrain->render(gl);
    m_leftTrain->render(gl);
    for(auto & object : m_dynamicObjects)
    {
        object->render(gl);
    }

    gl.glDisable(GL_BLEND);
    gl.glDisable(GL_DEPTH_TEST);
    gl.glDisable(GL_CULL_FACE);
}

LocalPlayer & World::localPlayer()
{
    return *m_localPlayer;
}

Train & World::playerTrain()
{
    return *m_rightTrain;
}

Train & World::enemyTrain()
{
    return *m_leftTrain;
}

Terrain &World::terrain()
{
    return *m_terrain;
}

Timer & World::timer()
{
    return m_game.timer();
}

LightManager &World::lightManager()
{
    return m_lightManager;
}

std::shared_ptr<BulletWorld> World::bulletWorld()
{
    return m_bulletWorld;
}

void World::scheduleAction(DeferredAction event)
{
    m_game.deferredActionHandler().scheduleAction(event);
}

} // namespace terminus
