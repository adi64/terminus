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
, m_deferredActionHandler(this)
, m_networkManager(*this)
, m_renderTrigger(std::unique_ptr<QTimer>(new QTimer()))
, m_paused(true)
, m_setupComplete(false)
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));

    ResourceManager::getInstance()->loadResources();
    SoundManager::getInstance()->playSound("music");

    SoundManager::getInstance()->playSound("music");

    m_networkManager.startServer(4711);

    startLocalGame();
    
    updateQMLData();
}

Game::~Game()
{

}

void terminus::Game::startLocalGame()
{
    m_world = std::unique_ptr<World>(new World(*this, false, true, std::chrono::system_clock::now().time_since_epoch().count()));
}

void Game::startNetworkGame(bool isPlayerOne)
{
    startNetworkGame(isPlayerOne, std::chrono::system_clock::now().time_since_epoch().count());
}

void Game::startNetworkGame(bool isPlayerOne, int terrainSeed)
{
    m_world = std::unique_ptr<World>(new World(*this, true, isPlayerOne, terrainSeed));
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

void Game::sync()
{
    if (m_world->playerTrain().travelledDistanceRelative() == 1.0f
            || m_world->enemyTrain().wagonAt(0)->isDisabled())
    {
        QMetaObject::invokeMethod(this, "winGame", Qt::DirectConnection);
        return;
    }
    else if (m_world->playerTrain().wagonAt(0)->isDisabled())
    {
        QMetaObject::invokeMethod(this, "loseGame", Qt::DirectConnection);
        return;
    }

    // check if it's our first frame
    if(!m_setupComplete)
    {
        m_setupComplete = true;
        m_paused = false;
        //TODO m_timeStamp->restart();
    }

    // process scheduled events
    m_deferredActionHandler.processDeferredActions();

    #ifdef Q_OS_MAC
        m_world->localPlayer().camera().setViewport(window()->width() * 2, window()->height() * 2);
    #else
        m_world->localPlayer().camera().setViewport(window()->width(), window()->height());
    #endif

    if(!m_paused)
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
        // trigger redraws periodically
        connect(m_renderTrigger.get(), &QTimer::timeout, win, &QQuickWindow::update);
        m_renderTrigger->start(1000 / 60);
    }
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
 * \brief Creates Data for QML. This is incredibly inefficient since QVariants cannot be edited only set.
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
        wagonMap.insert("currentCooldown", playerTrain.wagonAt(i)->cooldown());
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
