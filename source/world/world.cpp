#include "world.h"

#include <cmath>

#include <QDebug>
#include <QTime>
#include <QQuickWindow>

#include <util/gldebug.h>

#include <game.h>
#include <player/abstractplayer.h>
#include <player/aiplayer.h>
#include <player/localplayer.h>
#include <player/remoteplayer.h>
#include <resources/soundmanager.h>
#include <util/actionscheduler.h>
#include <world/camera.h>
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

World::World(Game & game, bool isNetworkGame, bool isPlayerOne, unsigned int terrainSeed)
: m_game(game)
, m_level(LevelConfiguration(terrainSeed))
, m_postprocessingManager(std::unique_ptr<PostprocessingManager>(new PostprocessingManager(*this)))
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

void World::render() const
{
    // render to g-buffer instead of to the screen
    m_postprocessingManager->gBufferFBO().bindFBO();


    printGlError(__FILE__, __LINE__);
    glViewport(0, 0, m_localPlayer->camera().viewport().x(), m_localPlayer->camera().viewport().y());

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_BLEND);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW); //TODO check if still necessary after geometry loader

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);

    printGlError(__FILE__, __LINE__);

//    m_skybox->render();
//    printGlError(__FILE__, __LINE__);
    m_terrain->render();
    printGlError(__FILE__, __LINE__);
    m_rightTrain->render();
    printGlError(__FILE__, __LINE__);
    m_leftTrain->render();
    printGlError(__FILE__, __LINE__);
    for(auto & object : m_dynamicObjects)
    {
        object->render();
    }
    printGlError(__FILE__, __LINE__);

    m_postprocessingManager->gBufferFBO().releaseFBO();

    // clear real framebuffer
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // disable stuff we don't need while post-processing
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // apply post-processing effects and render image to screen
    m_postprocessingManager->composeImage();
}

LocalPlayer & World::localPlayer()
{
    return *m_localPlayer;
}

AbstractPlayer & World::enemyPlayer()
{
    return *m_enemyPlayer;
}

Train & World::localPlayerTrain()
{
    return *(localPlayer().train());
}

Train & World::enemyPlayerTrain()
{
    return *(enemyPlayer().train());
}

Level & World::level()
{
    return m_level;
}

Timer & World::timer()
{
    return m_game.timer();
}

LightManager & World::lightManager()
{
    return m_lightManager;
}

QSize World::viewport() const
{
    return m_game.window()->size();
}

std::shared_ptr<BulletWorld> World::bulletWorld()
{
    return m_bulletWorld;
}

void World::scheduleAction(ActionScheduler::Action event)
{
    m_game.scheduler().scheduleAction(event);
}

NetworkManager & World::networkManager()
{
    return m_game.networkManager();
}

} // namespace terminus
