#include "game.h"

#include <memory>
#include <functional>

#include <QQuickView>
#include <QTimer>
#include <QTime>
#include <QVector3D>
#include <QApplication>

#include <eventhandler.h>
#include <deferredactionhandler.h>
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

        #ifdef Q_OS_MAC
            m_world->camera().setViewport(win->width()*2, win->height()*2);
        #else
            m_world->camera().setViewport(win->width(), win->height());
        #endif

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

}
