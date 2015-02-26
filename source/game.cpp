#include "game.h"

#include <assert.h>
#include <memory>
#include <functional>

#include <QQuickView>
#include <QTimer>
#include <QTime>
#include <QVector3D>
#include <QApplication>

#include <eventhandler.h>
#include <deferredactionhandler.h>
#include <player/localplayer.h>
#include <resources/resourcemanager.h>
#include <resources/soundmanager.h>
#include <world/world.h>

namespace terminus
{

Game::Game()
: m_timer(std::unique_ptr<QTimer>(new QTimer()))
, m_timeStamp(std::unique_ptr<QTime>(new QTime()))
, m_eventHandler(this)
, m_deferredActionHandler(this)
, m_paused(true)
, m_setupComplete(false)
{

    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));

    ResourceManager::getInstance()->loadResources();

    m_timeStamp->restart();

    m_world = std::unique_ptr<World>(new World(*this));

    SoundManager::getInstance()->playSound("music");
}

Game::~Game()
{

}

World & Game::world() const
{
    assert(m_world);
    return *m_world;
}

DeferredActionHandler & Game::deferredActionHandler()
{
    return m_deferredActionHandler;
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
    m_deferredActionHandler.processDeferredActions();

    #ifdef Q_OS_MAC
        m_world->localPlayer().camera().setViewport(window()->width()*2, window()->height()*2);
    #else
        m_world->localPlayer().camera().setViewport(window()->width(), window()->height());
    #endif

    auto elapsedMilliseconds = m_timeStamp->restart();
    if(m_paused)
    {
       elapsedMilliseconds = 0;
    }

    m_world->update(elapsedMilliseconds);
}

void Game::render()
{
    static bool glInitialized = false;
    if(!glInitialized)
    {
        m_gl.initializeOpenGLFunctions();
        glInitialized = true;
    }

    m_world->render(m_gl);
}

void Game::cleanup()
{
    qDebug("cleanup");
}

void Game::handleWindowChanged(QQuickWindow * win)
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
    m_eventHandler.keyPressEvent(key);
}

void Game::keyReleaseEvent(Qt::Key key)
{
    m_eventHandler.keyReleaseEvent(key);
}

void Game::mouseMoveEvent(qreal x, qreal y)
{
    m_eventHandler.mouseMoveEvent(x, y);
}

void Game::touchMoveEvent(qreal x, qreal y)
{
    m_eventHandler.touchMoveEvent(x, y);
}

void Game::gyroMoveEvent(qreal x, qreal y)
{
    m_eventHandler.gyroMoveEvent(x, y);
}

void Game::flickEvent(qreal startX, qreal x)
{
    m_eventHandler.flickEvent(startX, x);
}

void Game::flickReset()
{
    m_eventHandler.flickReset();
}

void Game::touchChargeFire()
{
    m_eventHandler.touchChargeFire();
}

void Game::touchFire()
{
    m_eventHandler.touchFire();
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
