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
#include <player/remoteplayer.h>

#include <world/camera.h>
#include <world/physics/abstractphysicsobject.h>
#include <world/physics/bulletworld.h>
#include <deferredactionhandler.h>

namespace terminus
{

World::World(Game & game, bool isNetworkGame, bool isPlayerOne, unsigned int terrainSeed)
: m_game(game)
, m_bulletWorld(std::shared_ptr<BulletWorld>(new BulletWorld))
, m_terrain(std::unique_ptr<Terrain>(new Terrain(*this, terrainSeed)))
{
    if(isPlayerOne)
    {
        m_playerTrain = std::unique_ptr<Train>(new Train(*this, m_terrain->playerTrack()));
        m_enemyTrain = std::unique_ptr<Train>(new Train(*this, m_terrain->enemyTrack()));
    }
    else
    {
        m_playerTrain = std::unique_ptr<Train>(new Train(*this, m_terrain->enemyTrack()));
        m_enemyTrain = std::unique_ptr<Train>(new Train(*this, m_terrain->playerTrack()));
    }

    m_localPlayer = std::unique_ptr<LocalPlayer>(new LocalPlayer(*this, m_playerTrain.get()));

    if(isNetworkGame)
    {

        m_enemyPlayer = std::unique_ptr<AbstractPlayer>(new RemotePlayer(*this, m_enemyTrain.get()));
    }
    else
    {
        m_enemyPlayer = std::unique_ptr<AbstractPlayer>(new AIPlayer(*this, m_enemyTrain.get(), m_playerTrain.get()));
        m_enemyTrain->follow(m_playerTrain.get());
    }

    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<RepairWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<RepairWagon>();
    m_playerTrain->addWagon<WeaponWagon>();

    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<RepairWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<RepairWagon>();

    m_skybox = std::unique_ptr<SkyBox>(new SkyBox(*this));

    addNode(m_playerTrain.get());
    addNode(m_enemyTrain.get());
    addNode(m_terrain.get());
    addNode(m_skybox.get());

    m_lightManager.add(Light::createAmbient({0.1f, 0.1f, 0.1f}));
    m_lightManager.add(Light::createDirectional({0.5f, 0.47f, 0.43f}, {-5.0, -1.0, 5.0}));
    m_lightManager.add(Light::createDirectional({0.4f, 0.43f, 0.5f}, {0.0, -1.0, 0.0}));

    localPlayer().camera().setEye(QVector3D(-30.0, 10.0, 20.0));
    localPlayer().camera().setCenter(QVector3D(0.0, 0.0, 10.0));
    localPlayer().camera().setUp(QVector3D(0.0, 1.0, 0.0));
    localPlayer().camera().lockToObject(m_playerTrain->wagonAt(0));
}

World::~World()
{

}

void World::addNode(AbstractGraphicsObject *node)
{
    m_objects.push_back(node);
}

void World::deleteNode(AbstractGraphicsObject *node)
{
    for(auto iterator = m_objects.begin(); iterator != m_objects.end(); ++iterator)
    {
        if(*iterator == node)
        {
            m_objects.erase(iterator);
            return;
        }
    }

    qDebug() << "Could not find node " << node;
}

void World::update()
{
    // physics - never give bullet negative step times
    m_bulletWorld->stepSimulation(fmax(m_game.timer().get("frameTimer") / 1000.f, 0.f), 10);

    for(auto object : m_objects)
    {
        object->update();
    }

    m_enemyPlayer->update();
    m_localPlayer->update();

    // camera updates after all other nodes because it can follow the position of other nodes
    m_localPlayer->camera().update();
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

    for(auto node : m_objects)
    {
        node->render(gl);
    }

    gl.glDisable(GL_BLEND);
    gl.glDisable(GL_DEPTH_TEST);
    gl.glDisable(GL_CULL_FACE);
}

LocalPlayer & World::localPlayer()
{
    return *m_localPlayer;
}

AbstractPlayer &World::enemyPlayer()
{
    return *m_enemyPlayer;
}

Train & World::playerTrain()
{
    return *m_playerTrain;
}

Train & World::enemyTrain()
{
    return *m_enemyTrain;
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

NetworkManager & World::networkManager()
{
    return m_game.networkManager();
}

} // namespace terminus
