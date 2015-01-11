#include "game.h"

#include <memory>

#include <QQuickView>
#include <QTimer>
#include <QTime>
#include <QVector3D>
#include <QApplication>

#include "scene.h"
#include "resources/resourcemanager.h"
#include "resources/soundmanager.h"
#include "train.h"
#include "terrain.h"
#include "skybox.h"
#include "eventhandler.h"

#include "resources/resourcemanager.h"
#include "wagons/enginewagon.h"
#include "wagons/weaponwagon.h"

namespace terminus
{

Game::Game()
: m_timer(new QTimer())
, m_timeStamp(new QTime())
, m_eventHandler(std::unique_ptr<EventHandler>(new EventHandler(this)))
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));

    ResourceManager::getInstance()->loadResources();

    m_timeStamp->start();


    // Set up bullet
    // Build the broadphase
    m_bullet_broadphase = std::unique_ptr<btBroadphaseInterface>(new btDbvtBroadphase());

    // Set up the collision configuration and dispatcher
    m_bullet_collisionConfiguration = std::unique_ptr<btDefaultCollisionConfiguration>(new btDefaultCollisionConfiguration());
    m_bullet_dispatcher = std::unique_ptr<btCollisionDispatcher>(new btCollisionDispatcher(m_bullet_collisionConfiguration.get()));

    // The actual physics solver
    m_bullet_solver = std::unique_ptr<btSequentialImpulseConstraintSolver>(new btSequentialImpulseConstraintSolver);

    // The world.
    m_bullet_dynamicsWorld = std::unique_ptr<btDiscreteDynamicsWorld>(
                new btDiscreteDynamicsWorld(
                    m_bullet_dispatcher.get(),
                    m_bullet_broadphase.get(),
                    m_bullet_solver.get(),
                    m_bullet_collisionConfiguration.get()
                    )
                );
    m_bullet_dynamicsWorld->setGravity(btVector3(0, -10, 0));

    m_scene = std::shared_ptr<Scene>(new Scene(m_bullet_dynamicsWorld));

    SoundManager::getInstance()->playBackgroundMusic();

    m_terrain = std::unique_ptr<Terrain>(new Terrain(m_scene));

    m_playerTrain = std::unique_ptr<Train>(new Train(m_scene, m_terrain->playerTrack()));
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->addWagon<WeaponWagon>();
    m_playerTrain->moveWagon(1, 2);

    m_enemyTrain = std::unique_ptr<Train>(new Train(m_scene, m_terrain->enemyTrack()));
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();
    m_enemyTrain->addWagon<WeaponWagon>();

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
    // do not delete this destructor, even if it is empty
    // otherwise std::shared_ptr<IncompleteType> in the header will break
    //
    // ... :D
}

void Game::sync()
{
    auto elapsedMilliseconds = m_timeStamp->restart();

    // physics
    m_bullet_dynamicsWorld->stepSimulation((float)elapsedMilliseconds / 1000.0f, 10);

    //TODO  // m_scene->setViewportSize(window()->size() * window()->devicePixelRatio());
    m_scene->camera().setViewport(window()->width(), window()->height());

    m_scene->update(elapsedMilliseconds);
}

void Game::render()
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
        connect(m_timer, &QTimer::timeout, win, &QQuickWindow::update);
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

void Game::flickEvent(qreal velo)
{
    m_eventHandler->flickEvent(velo);
}

Scene *Game::scene() const
{
    return m_scene.get();
}

Train *Game::playerTrain() const
{
    return m_playerTrain.get();
}

}
