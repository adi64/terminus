#include "game.h"

#include <assert.h>
#include <memory>
#include <chrono>

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
#include <network/networkmanager.h>

namespace terminus
{

Game::Game()
: m_eventHandler(this)
, m_networkManager(*this)
, m_renderTrigger(std::unique_ptr<QTimer>(new QTimer()))
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));

    ResourceManager::getInstance()->loadResources();
}

Game::~Game()
{

}

void Game::startLocalGame()
{
    createWorld(false, true, std::chrono::system_clock::now().time_since_epoch().count());
}

void Game::hostNetworkGame()
{
    m_networkManager.startServer(defaultPort);
    createWorld(true, true, std::chrono::system_clock::now().time_since_epoch().count());
    setPaused(true);
}

void Game::joinNetworkGame(QString host)
{
    m_networkManager.startClient(host, defaultPort);
}

void Game::createWorld(bool isNetworkGame, bool isPlayerOne, int terrainSeed)
{
    m_world = std::unique_ptr<World>(new World(*this, isNetworkGame, isPlayerOne, terrainSeed));
    updateQMLData();
    QMetaObject::invokeMethod(this, "loadUI", Qt::AutoConnection, Q_ARG(QVariant, QVariant(!isPlayerOne)));
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

NetworkManager &Game::networkManager()
{
    return m_networkManager;
}

Timer & Game::timer()
{
    return m_timer;
}

void Game::buttonInput(int type)
{
    m_eventHandler.buttonInput(type);
}

void Game::keyInput(Qt::Key key)
{
    m_eventHandler.keyInput(key);
}

void Game::moveInput(int type, qreal x, qreal y)
{
    m_eventHandler.moveInput(type, x, y);
}

void Game::touchInput(qreal oldx, qreal oldy, qreal x, qreal y)
{
    m_eventHandler.touchInput(oldx, oldy, x, y);
}

void Game::sync()
{
    // process scheduled events
    m_deferredActionHandler.processDeferredActions();

    if(!m_world)
    {
        return;
    }

    // check winning / losing condition
    if (m_world->localPlayerTrain().travelledDistanceRelative() == 1.0f
            || m_world->enemyPlayerTrain().wagonAt(0)->isDisabled())
    {
        QMetaObject::invokeMethod(this, "winGame", Qt::AutoConnection);
        return;
    }
    else if (m_world->localPlayerTrain().wagonAt(0)->isDisabled())
    {
        QMetaObject::invokeMethod(this, "loseGame", Qt::AutoConnection);
        return;
    }

    #ifdef Q_OS_MAC
        m_world->localPlayer().camera().setViewport(window()->width() * 2, window()->height() * 2);
    #else
        m_world->localPlayer().camera().setViewport(window()->width(), window()->height());
    #endif

    if(!m_timer.isPaused())
    {
       m_world->update();
    }
    updateQMLData();

    m_timer.adjust("frameTimer", 0);
}

void Game::render()
{
    static bool glInitialized = false;
    if(!glInitialized)
    {
        m_gl.initializeOpenGLFunctions();
        glInitialized = true;
    }

    if(m_world)
    {
        m_world->render(m_gl);
    }
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
        // trigger redraws periodically
        connect(m_renderTrigger.get(), &QTimer::timeout, win, &QQuickWindow::update);
        m_renderTrigger->start(1000 / 60);
    }
}

void Game::setPaused(bool paused)
{
    m_timer.pause(paused);
}

void Game::togglePaused()
{
    m_timer.pause();
}

/*!
 * \brief Creates Data for QML. This is incredibly inefficient since QVariants cannot be edited only set.
 */
void Game::updateQMLData()
{
    auto& playerTrain = m_world->localPlayerTrain();
    QList<QVariant> playerWagonList;
    for (unsigned int i = 0; i < playerTrain.size(); i++)
    {
        QMap<QString, QVariant> wagonMap;
        wagonMap.insert("type", playerTrain.wagonAt(i)->wagonType());
        wagonMap.insert("currentHealth", playerTrain.wagonAt(i)->currentHealth());
        wagonMap.insert("maxHealth", playerTrain.wagonAt(i)->maxHealth());
        wagonMap.insert("currentCooldown", playerTrain.wagonAt(i)->cooldown());
        wagonMap.insert("isDisabled", playerTrain.wagonAt(i)->isDisabled());
        playerWagonList.push_back(wagonMap);
    }

    auto& enemyTrain = m_world->enemyPlayerTrain();
    QList<QVariant> enemyWagonList;
    for (unsigned int i = 0; i < enemyTrain.size(); i++)
    {
        QMap<QString, QVariant> wagonMap;
        wagonMap.insert("type", enemyTrain.wagonAt(i)->wagonType());
        wagonMap.insert("currentHealth", enemyTrain.wagonAt(i)->currentHealth());
        wagonMap.insert("maxHealth", enemyTrain.wagonAt(i)->maxHealth());
        wagonMap.insert("currentCooldown", enemyTrain.wagonAt(i)->cooldown());
        wagonMap.insert("isDisabled", enemyTrain.wagonAt(i)->isDisabled());
        QVariant wagon(wagonMap);
        enemyWagonList.push_back(wagon);
    }

    QMap<QString, QVariant> playerTrainMap;
    playerTrainMap.insert("totalWagons", playerTrain.size());
    playerTrainMap.insert("currentWagon", m_world->localPlayer().selectedWagonIndex());
    playerTrainMap.insert("wagons", playerWagonList);
    float progress = playerTrain.travelledDistanceRelative();
    playerTrainMap.insert("progress", progress);

    QMap<QString, QVariant> enemyTrainMap;
    enemyTrainMap.insert("totalWagons", playerTrain.size());
    enemyTrainMap.insert("wagons", enemyWagonList);

    QMap<QString, QVariant> dataMap;
    dataMap.insert("PlayerTrain", playerTrainMap);
    dataMap.insert("EnemyTrain", enemyTrainMap);
    m_qmlData.setValue(dataMap);
    emit qmlDataChanged();
}

QVariant & Game::qmlData()
{
    return m_qmlData;
}

}
