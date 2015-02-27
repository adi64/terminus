#include "game.h"

#include <assert.h>
#include <memory>

#include <QQuickView>
#include <QApplication>
#include <QTimer>
#include <QTime>
#include <QVariant>
#include <QMap>
#include <QList>

#include <player/aiplayer.h>

#include <eventhandler.h>
#include <deferredactionhandler.h>
#include <player/localplayer.h>
#include <resources/resourcemanager.h>
#include <resources/soundmanager.h>
#include <world/world.h>

namespace terminus
{

Game::Game()
: m_eventHandler(this)
, m_deferredActionHandler(this)
, m_timer(std::unique_ptr<QTimer>(new QTimer()))
, m_timeStamp(std::unique_ptr<QTime>(new QTime()))
, m_paused(true)
, m_setupComplete(false)
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));

    ResourceManager::getInstance()->loadResources();
    SoundManager::getInstance()->playSound("music");

    m_timeStamp->restart();

    m_world = std::unique_ptr<World>(new World(*this));

    updateQMLData();
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
    updateQMLData();
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

void Game::switchToNextWagon()
{
    m_eventHandler.switchToNextWagon();
}

void Game::switchToPreviousWagon()
{
    m_eventHandler.switchToPreviousWagon();
}

/*void Game::flickEvent(qreal startX, qreal x)
{
    m_eventHandler.flickEvent(startX, x);
}

void Game::flickReset()
{
    m_eventHandler.flickReset();
}*/

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

/*!
 * \brief Creates Data for QML. This is incredibly inefficient since QVariants cant be edited only set.
 */
void Game::updateQMLData()
{
    auto& playerTrain = m_world->playerTrain();
    QList<QVariant> playerWagonList;
    for (unsigned int i = 0; i < playerTrain.size(); i++)
    {
        QMap<QString, QVariant> wagonMap;
        wagonMap.insert("type", playerTrain.wagonAt(i)->wagonType());
        wagonMap.insert("currentHealth", playerTrain.wagonAt(i)->currentHealth());
        wagonMap.insert("maxHealth", playerTrain.wagonAt(i)->maxHealth());
        //wagonMap.insert("currentCooldown", 0);
        //wagonMap.insert("maxCooldown", 0);
        wagonMap.insert("isDisabled", playerTrain.wagonAt(i)->isDisabled());
        playerWagonList.push_back(wagonMap);
    }

    auto& enemyTrain = m_world->enemyTrain();
    QList<QVariant> enemyWagonList;
    for (unsigned int i = 0; i < enemyTrain.size(); i++)
    {
        QMap<QString, QVariant> wagonMap;
        wagonMap.insert("type", enemyTrain.wagonAt(i)->wagonType());
        wagonMap.insert("currentHealth", enemyTrain.wagonAt(i)->currentHealth());
        wagonMap.insert("maxHealth", enemyTrain.wagonAt(i)->maxHealth());
        wagonMap.insert("currentCooldown", 0);
        wagonMap.insert("maxCooldown", 0);
        wagonMap.insert("isDisabled", enemyTrain.wagonAt(i)->isDisabled());
        QVariant wagon(wagonMap);
        enemyWagonList.push_back(wagon);
    }

    QMap<QString, QVariant> playerTrainMap;
    playerTrainMap.insert("totalWagons", playerTrain.size());
    playerTrainMap.insert("currentWagon", m_world->localPlayer().selectedWagonIndex());
    playerTrainMap.insert("wagons", playerWagonList);

    QMap<QString, QVariant> enemyTrainMap;
    enemyTrainMap.insert("totalWagons", playerTrain.size());
    enemyTrainMap.insert("wagons", enemyWagonList);

    QMap<QString, QVariant> dataMap;
    dataMap.insert("PlayerTrain", playerTrainMap);
    dataMap.insert("EnemyTrain", enemyTrainMap);
    m_qmlData.setValue(dataMap);
    qmlDataChanged();
}

QVariant & Game::qmlData()
{
    return m_qmlData;
}

}
