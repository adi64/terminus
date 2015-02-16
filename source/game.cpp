#include "game.h"

#include <memory>
#include <functional>

#include <QQuickView>
#include <QTimer>
#include <QTime>
#include <QVector3D>
#include <QApplication>

#include <resources/lightmanager.h>
#include <resources/resourcemanager.h>
#include <resources/resourcemanager.h>
#include <resources/soundmanager.h>

#include <world/scene.h>
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

#include "eventhandler.h"
#include "deferredactionhandler.h"

namespace terminus
{

Game::Game()
: m_timer(std::unique_ptr<QTimer>(new QTimer()))
, m_timeStamp(std::shared_ptr<QTime>(new QTime()))
, m_eventHandler(std::unique_ptr<EventHandler>(new EventHandler(this)))
, m_deferredActionHandler(std::shared_ptr<DeferredActionHandler>(new DeferredActionHandler(this)))
, m_lightManager(std::unique_ptr<LightManager>(new LightManager))
, m_paused(true)
, m_setupComplete(false)
{

    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));

    ResourceManager::getInstance()->loadResources();

    m_timeStamp->restart();

    setupBulletWorld();

    m_scene = std::shared_ptr<Scene>(new Scene(m_bullet_dynamicsWorld, m_deferredActionHandler));

    SoundManager::getInstance()->playSound("music");

    m_terrain = std::unique_ptr<Terrain>(new Terrain(m_scene));

    m_playerTrain = std::shared_ptr<Train>(new Train(m_scene, m_terrain->playerTrack()));
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<RepairWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<RepairWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<RepairWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();

    m_enemyTrain = std::shared_ptr<Train>(new Train(m_scene, m_terrain->enemyTrack()));
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<RepairWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<RepairWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->follow(m_playerTrain);

    m_localPlayer = std::unique_ptr<LocalPlayer>(new LocalPlayer(m_playerTrain));
    m_aiPlayer = std::unique_ptr<AIPlayer>(new AIPlayer(m_enemyTrain, m_playerTrain));

    m_scene->setActiveCamera(m_localPlayer->camera());

    m_skybox = std::unique_ptr<SkyBox>(new SkyBox(m_scene));

    m_scene->setInitialTimeStamp(m_timeStamp);

    m_scene->addNode(m_playerTrain.get());
    m_scene->addNode(m_enemyTrain.get());
    m_scene->addNode(m_terrain.get());
    m_scene->addNode(m_skybox.get());

    m_scene->camera().setEye(QVector3D(-30.0, 10.0, 20.0));
    m_scene->camera().setCenter(QVector3D(0.0, 0.0, 10.0));
    m_scene->camera().setUp(QVector3D(0.0, 1.0, 0.0));
    m_scene->camera().lockToObject(m_playerTrain->wagonAt(0));
}

Game::~Game()
{

}

void Game::sync()
{
    // check if it's our first frame
    if(!m_setupComplete)
    {
        m_setupComplete = true;
        m_paused = false;
        m_timeStamp->restart();
    }

    // process scheduled events
    m_deferredActionHandler->processDeferredActions();

    auto elapsedMilliseconds = m_timeStamp->restart();
    if(m_paused)
    {
       elapsedMilliseconds = 0;
    }

    // physics
    m_bullet_dynamicsWorld->stepSimulation((float)elapsedMilliseconds / 1000.0f, 10);

    #ifdef Q_OS_MAC
        m_scene->camera().setViewport(window()->width()*2, window()->height()*2);
    #else
        m_scene->camera().setViewport(window()->width(), window()->height());
    #endif

    m_scene->update(elapsedMilliseconds);

    m_aiPlayer->update(elapsedMilliseconds);
    m_localPlayer->update(elapsedMilliseconds);
}

void Game::render() const
{
    m_scene->render();
}

void Game::cleanup()
{
    qDebug("cleanup");
}

void Game::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, SIGNAL(beforeRendering()), this, SLOT(render()), Qt::DirectConnection);
        connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);
        connect(win, SIGNAL(sceneGraphInvalidated()), this, SLOT(cleanup()), Qt::DirectConnection);
        // If we allow QML to do the clearing, they would clear what we paint
        // and nothing would show.
        win->setClearBeforeRendering(false);

        // force redraw
        connect(m_timer.get(), &QTimer::timeout, win, &QQuickWindow::update);
        m_timer->start(1000 / 60);
    }
}

void Game::keyPressEvent(Qt::Key key)
{
    m_eventHandler->keyPressEvent(key);
}

void Game::keyReleaseEvent(Qt::Key key)
{
    m_eventHandler->keyReleaseEvent(key);
}

void Game::mouseMoveEvent(qreal x, qreal y)
{
    m_eventHandler->mouseMoveEvent(x, y);
}

void Game::touchMoveEvent(qreal x, qreal y)
{
    m_eventHandler->touchMoveEvent(x, y);
}

void Game::gyroMoveEvent(qreal x, qreal y)
{
    m_eventHandler->gyroMoveEvent(x, y);
}

void Game::flickEvent(qreal startX, qreal x)
{
    m_eventHandler->flickEvent(startX, x);
}

void Game::flickReset()
{
    m_eventHandler->flickReset();
}

void Game::touchChargeFire()
{
    m_eventHandler->touchChargeFire();
}

void Game::touchFire()
{
    m_eventHandler->touchFire();
}

void Game::setPaused(bool paused)
{
    m_paused = paused;
}

void Game::togglePaused()
{
    m_paused = !m_paused;
}

void Game::setupBulletWorld()
{
    // these objects must not be deleted before m_bullet_dynamicsWorld
    // -- so as a temporary hack, we won't delete them at all

    // Build the broadphase
    m_bullet_broadphase = new btDbvtBroadphase();

    // Set up the collision configuration and dispatcher
    m_bullet_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_bullet_dispatcher = new btCollisionDispatcher(m_bullet_collisionConfiguration);

    // The actual physics solver
    m_bullet_solver = new btSequentialImpulseConstraintSolver;

    // The world.
    m_bullet_dynamicsWorld = std::shared_ptr<btDiscreteDynamicsWorld>(
                new btDiscreteDynamicsWorld(
                    m_bullet_dispatcher,
                    m_bullet_broadphase,
                    m_bullet_solver,
                    m_bullet_collisionConfiguration
                    )
                );

    m_bullet_dynamicsWorld->setGravity(btVector3(0.0f, -9.81f, 0.0f));

    // set world user info (void*) to pointer to this game instance
    // so we can (indirectly) call a member of Game without having global state or a singleton
    m_bullet_dynamicsWorld->setInternalTickCallback(&Game::btStaticTickCallback);
    m_bullet_dynamicsWorld->setWorldUserInfo(static_cast<void*>(this));
}

Scene *Game::scene() const
{
    return m_scene.get();
}

Train *Game::playerTrain() const
{
    return m_playerTrain.get();
}

AbstractPlayer *Game::localPlayer() const
{
    return m_localPlayer.get();
}

void Game::btTickCallback(btDynamicsWorld *world, btScalar timeStep)
{
    int numManifolds = world->getDispatcher()->getNumManifolds();

    for (int i=0; i < numManifolds; ++i)
    {
        auto contactManifold =  world->getDispatcher()->getManifoldByIndexInternal(i);
        auto body0 = contactManifold->getBody0();
        auto body1 = contactManifold->getBody1();

        auto numContacts = contactManifold->getNumContacts();

        if(numContacts > 0)
        {
            auto physicsObject0 = m_scene->getGraphicsObjectForCollisionObject(body0);
            auto physicsObject1 = m_scene->getGraphicsObjectForCollisionObject(body1);

            if(physicsObject0 != nullptr && physicsObject1 != nullptr)
            {
                physicsObject0->onCollisionWith(physicsObject1);
                physicsObject1->onCollisionWith(physicsObject0);
            }
        }
    }

}

void Game::btStaticTickCallback(btDynamicsWorld *world, btScalar timeStep)
{
    // retrieve instance pointer from user info (void*)
    auto gameInstance = static_cast<Game*>(world->getWorldUserInfo());
    gameInstance->btTickCallback(world, timeStep);
}

}
