#include "game.h"

#include <cassert>
#include <chrono>
#include <memory>
#include <queue>

#include <QApplication>
#include <QList>
#include <QMap>
#include <QQuickView>
#include <QTimer>
#include <QVariant>

#include <eventhandler.h>
#include <network/networkmanager.h>
#include <player/aiplayer.h>
#include <player/localplayer.h>
#include <resources/resourcemanager.h>
#include <util/actionscheduler.h>
#include <world/world.h>


namespace terminus
{

Game::Game()
: m_eventHandler(this)
, m_networkManager(*this)
, m_window(nullptr)
, m_renderTrigger(std::unique_ptr<QTimer>(new QTimer()))
, m_isGLInitialized(false)
, m_isUIActive(false) // is set to true on create world
, m_showFPS(false)
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));

    ResourceManager::getInstance()->loadResources();
}

Game::~Game()
{
    disconnectSignals();
}

void Game::startLocalGame()
{
    createWorld(false, true, std::chrono::system_clock::now().time_since_epoch().count());
}

void Game::hostNetworkGame()
{
    m_networkManager.startServer(defaultPort);
    createWorld(true, true, std::chrono::system_clock::now().time_since_epoch().count());
}

void Game::joinNetworkGame(QString host)
{
    m_networkManager.startClient(host, defaultPort);
}

void Game::createWorld(bool isNetworkGame, bool isPlayerOne, long long terrainSeed)
{
    m_timer.pause(isNetworkGame);
    m_timer.adjust(0);
    m_isPlayerOne = isPlayerOne;

    QList<WagonType> playerTrain;
    for (auto & wagon : m_qmlData.toMap()["playerTrain"].toList())
    {
        playerTrain.push_back(WagonType(wagon.toMap()["type"].toInt()));
    }

    QList<WagonType> enemyTrain;
    for (auto & wagon : m_qmlData.toMap()["enemyTrain"].toList())
    {
        enemyTrain.push_back(WagonType(wagon.toMap()["type"].toInt()));
    }

    m_world = std::unique_ptr<World>(new World(*this, isNetworkGame, isPlayerOne, terrainSeed, playerTrain, enemyTrain));
    showUI();
}

void Game::endGame(bool localPlayerWins, bool showMessage)
{
    disconnectSignals();
    m_networkManager.sendGameEndedCommand(!localPlayerWins && showMessage);
    if (showMessage && localPlayerWins)
    {
        QMetaObject::invokeMethod(this, "winGame", Qt::AutoConnection);
    }
    else if (showMessage && !localPlayerWins)
    {
        QMetaObject::invokeMethod(this, "loseGame", Qt::AutoConnection);
    }
    else
    {
        QMetaObject::invokeMethod(this, "stopGame", Qt::AutoConnection);
    }
}

void Game::toggleUI()
{
    if (m_isUIActive)
    {
        hideUI();
    }
    else
    {
        showUI();
    }
}

void Game::showUI()
{
    m_isUIActive = true;
    updateQMLData();
    QString uiFile = "qrc:/source/qml/UserInterface.qml";
    QMetaObject::invokeMethod(this, "loadUI", Qt::AutoConnection, Q_ARG(QVariant, uiFile), Q_ARG(QVariant, !m_isPlayerOne));
}

void Game::hideUI()
{
    m_isUIActive = false;
    QString uiFile = "";
    QMetaObject::invokeMethod(this, "loadUI", Qt::AutoConnection, Q_ARG(QVariant, uiFile), Q_ARG(QVariant, !m_isPlayerOne));
}

void Game::toggleFPS()
{
    m_showFPS = !m_showFPS;
}

World & Game::world() const
{
    assert(m_world);
    return *m_world;
}

ActionScheduler & Game::scheduler()
{
    return m_scheduler;
}

NetworkManager & Game::networkManager()
{
    return m_networkManager;
}

Timer & Game::timer()
{
    return m_timer;
}

void Game::buttonInput(int type)
{
    if(m_world)
    {
        m_eventHandler.buttonInput(type);;
    }
}

void Game::keyInput(Qt::Key key)
{
    if(m_world)
    {
        m_eventHandler.keyInput(key);
    }
}

void Game::moveInput(int type, qreal x, qreal y)
{
    if(m_world)
    {
        m_eventHandler.moveInput(type, x, y);
    }
}

void Game::touchInput(qreal oldx, qreal oldy, qreal x, qreal y)
{
    if(m_world)
    {
        m_eventHandler.touchInput(oldx, oldy, x, y);
    }
}

void Game::sync()
{
    // process scheduled events
    m_scheduler.executeActions();

    if(!m_world)
    {
        return;
    }

    #ifdef Q_OS_MAC
        m_world->localPlayer().camera().setViewport(window()->width() * 2, window()->height() * 2);
    #else
        m_world->localPlayer().camera().setViewport(window()->width(), window()->height());
    #endif

    if (!m_timer.isPaused())
    {
       m_world->update();
    }

    m_networkManager.update();

    if (m_isUIActive)
    {
        updateQMLData();
    }

    m_timer.adjust("frameTimer", 0);
}

void Game::render()
{
    if (!m_isGLInitialized)
    {
        m_gl.initializeOpenGLFunctions();
        m_isGLInitialized = true;
    }

    if (m_world)
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
    connectSignals(win);
    if (m_window)
    {
        // If we allow QML to do the clearing, they would clear what we paint
        // and nothing would show.
        m_window->setClearBeforeRendering(false);
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
    m_networkManager.sendPauseCommand(m_timer.isPaused());
}

void Game::connectSignals(QQuickWindow *win)
{
    disconnectSignals();
    m_window = win;
    if (m_window)
    {
        connect(m_window, SIGNAL(beforeRendering()), this, SLOT(render()), Qt::DirectConnection);
        connect(m_window, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);
        connect(m_window, SIGNAL(sceneGraphInvalidated()), this, SLOT(cleanup()), Qt::DirectConnection);
    }
}

void Game::disconnectSignals()
{
    if(m_window)
    {
        disconnect(m_window, SIGNAL(beforeRendering()), this, SLOT(render()));
        disconnect(m_window, SIGNAL(beforeSynchronizing()), this, SLOT(sync()));
        disconnect(m_window, SIGNAL(sceneGraphInvalidated()), this, SLOT(cleanup()));
    }
}

/*!
 * \brief Creates Data for QML. This is somewhat inefficient since QVariants cannot be edited only set.
 */
void Game::updateQMLData()
{
    auto& playerTrain = m_world->localPlayerTrain();
    QList<QVariant> playerTrainList;
    for (unsigned int i = 0; i < playerTrain.size(); i++)
    {
        playerTrainList.push_back(playerTrain.wagonAt(i)->getStatus());
    }

    auto& enemyTrain = m_world->enemyPlayerTrain();
    QList<QVariant> enemyTrainList;
    for (unsigned int i = 0; i < enemyTrain.size(); i++)
    {
        enemyTrainList.push_back(enemyTrain.wagonAt(i)->getStatus());
    }

    auto fps = m_showFPS == true ? updateFPS() : -1;

    QMap<QString, QVariant> dataMap = {
        std::make_pair("currentWagon", m_world->localPlayer().selectedWagonIndex()),
        std::make_pair("progress", playerTrain.travelledDistanceRelative()),
        std::make_pair("playerTrain", playerTrainList),
        std::make_pair("enemyTrain", enemyTrainList),
        std::make_pair("fps", fps)
    };
    m_qmlData.setValue(dataMap);
    emit qmlDataChanged();
}

float Game::updateFPS()
{
    std::string timerName("fpsCounter");

    if (!m_timer.isAllocated(timerName)) {
        m_timer.allocateTimer(timerName);
    }

    auto now = m_timer.get(timerName);

    m_frameTimes.push(now);
    if (m_frameTimes.size() > 10)
    {
        m_frameTimes.pop();
        auto duration = now - m_frameTimes.front();
        return 1000.f / (duration / m_frameTimes.size());
    }
    return -1.f;
}

QVariant & Game::qmlData()
{
    return m_qmlData;
}

void Game::writeQmlData(QVariant qmlData)
{
    m_qmlData = qmlData;
}

}
