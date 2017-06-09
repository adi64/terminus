#include "game.h"

#include <cmath>

#include <QDebug>
#include <QTime>
#include <QQuickWindow>

#include <util/gldebug.h>

#include <application.h>
#include <player/abstractplayer.h>
#include <player/aiplayer.h>
#include <player/localplayer.h>
#include <player/remoteplayer.h>
#include <resources/soundmanager.h>
#include <util/actionscheduler.h>
#include <player/camera.h>
#include <world/postprocessing/postprocessingmanager.h>
#include <world/drawables/projectile.h>
#include <world/drawables/skybox.h>
#include <world/drawables/terrain.h>
#include <world/drawables/train/train.h>
#include <world/drawables/train/wagons/enginewagon.h>
#include <world/drawables/train/wagons/repairwagon.h>
#include <world/drawables/train/wagons/weaponwagon.h>
#include <world/physics/abstractphysicsobject.h>
#include <world/physics/bulletworld.h>


namespace terminus
{

Game::Game(Application & game, bool isNetworkGame, bool isPlayerOne, unsigned int terrainSeed)
: m_game(game)
, m_level(LevelConfiguration(terrainSeed))
, m_bulletWorld(std::shared_ptr<BulletWorld>(new BulletWorld))
, m_skybox(nullptr)
{
    m_level.generateLevel();
    m_terrain = std::unique_ptr<Terrain>(new Terrain(*this, m_level));
    m_terrain->configureWith(m_level);
    m_rightTrain = std::unique_ptr<Train>(new Train(*this, &(m_terrain->rightTrack())));
    m_leftTrain = std::unique_ptr<Train>(new Train(*this, &(m_terrain->leftTrack())));

    Train* playerTrain = nullptr;
    Train* enemyTrain = nullptr;

    if(isPlayerOne)
    {
        playerTrain = m_rightTrain.get();
        enemyTrain = m_leftTrain.get();
    }
    else
    {
        playerTrain = m_leftTrain.get();
        enemyTrain = m_rightTrain.get();
    }

    m_localPlayer = std::unique_ptr<LocalPlayer>(new LocalPlayer(*this, playerTrain));

    if(isNetworkGame)
    {
        m_enemyPlayer = std::unique_ptr<AbstractPlayer>(new RemotePlayer(*this, enemyTrain));
    }
    else
    {
        m_enemyPlayer = std::unique_ptr<AbstractPlayer>(new AIPlayer(*this, enemyTrain, playerTrain));
    }

    m_leftTrain->addWagon<WeaponWagon>();
    m_leftTrain->addWagon<WeaponWagon>();
    m_leftTrain->addWagon<RepairWagon>();
    m_leftTrain->addWagon<WeaponWagon>();
    m_leftTrain->addWagon<WeaponWagon>();
    m_leftTrain->addWagon<RepairWagon>();
    m_leftTrain->addWagon<WeaponWagon>();

    m_rightTrain->addWagon<WeaponWagon>();
    m_rightTrain->addWagon<WeaponWagon>();
    m_rightTrain->addWagon<RepairWagon>();
    m_rightTrain->addWagon<WeaponWagon>();
    m_rightTrain->addWagon<WeaponWagon>();
    m_rightTrain->addWagon<WeaponWagon>();
    m_rightTrain->addWagon<RepairWagon>();

    localPlayer().camera().state().setEye(QVector3D(-30.0, 10.0, 20.0));
    localPlayer().camera().state().setCenter(QVector3D(0.0, 0.0, 10.0));
    localPlayer().camera().state().setUp(QVector3D(0.0, 1.0, 0.0));

    m_lightAmbient = m_renderer.allocateLight(Light::createAmbient({0.1f, 0.1f, 0.1f}));
    m_lightSun = m_renderer.allocateLight(Light::createDirectional({0.5f, 0.47f, 0.43f}, {-5.0, -1.0, 5.0}));
    m_lightSky = m_renderer.allocateLight(Light::createDirectional({0.4f, 0.43f, 0.5f}, {0.0, -1.0, 0.0}));
}

Game::~Game()
{

}

void Game::addObject(GameObject * node)
{
    m_dynamicObjects.push_back(std::unique_ptr<GameObject>(node));
}

void Game::deleteObject(GameObject * object)
{
    m_dynamicObjects.remove_if(
        [object](const std::unique_ptr<GameObject> & candidate)
        {
            return candidate.get() == object;
        });
}

void Game::update()
{
    // physics - never give bullet negative step times
    m_bulletWorld->stepSimulation(fmax(m_game.timer().get("frameTimer") / 1000.f, 0.f), 10);

//    m_skybox->update();
    m_terrain->update();
    m_rightTrain->update();
    m_leftTrain->update();
    for(auto & object : m_dynamicObjects)
    {
        object->update();
    }

    m_enemyPlayer->update();
    m_localPlayer->update();

    if(m_enemyPlayer->hasWon() || m_localPlayer->hasLost())
    {
        m_game.endGame(false, true);
    }
    else if(m_localPlayer->hasWon() || m_enemyPlayer->hasLost())
    {
        m_game.endGame(true, true);
    }
}

void Game::render() const
{
    m_renderer.render(m_localPlayer->camera().state());
}

LocalPlayer & Game::localPlayer()
{
    return *m_localPlayer;
}

AbstractPlayer & Game::enemyPlayer()
{
    return *m_enemyPlayer;
}

Train & Game::localPlayerTrain()
{
    return *(localPlayer().train());
}

Train & Game::enemyPlayerTrain()
{
    return *(enemyPlayer().train());
}

Level & Game::level()
{
    return m_level;
}

Timer & Game::timer()
{
    return m_game.timer();
}

Renderer & Game::renderer()
{
    return m_renderer;
}

std::shared_ptr<BulletWorld> Game::bulletWorld()
{
    return m_bulletWorld;
}

void Game::scheduleAction(ActionScheduler::Action event)
{
    m_game.scheduler().scheduleAction(event);
}

NetworkManager & Game::networkManager()
{
    return m_game.networkManager();
}

} // namespace terminus
