#include "application.h"

#include <cassert>
#include <chrono>
#include <memory>

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
#include <world/game.h>


namespace terminus
{

Application::Application()
: m_eventHandler(this)
, m_networkManager(*this)
, m_window(nullptr)
, m_renderTrigger(std::unique_ptr<QTimer>(new QTimer()))
, m_isUIActive(false) // is set to true on create world
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));

    ResourceManager::getInstance()->loadResources();
}

Application::~Application()
{
    disconnectSignals();
}

void Application::startLocalGame()
{
    createWorld(false, true, std::chrono::system_clock::now().time_since_epoch().count());
}

void Application::hostNetworkGame()
{
    m_networkManager.startServer(defaultPort);
    createWorld(true, true, std::chrono::system_clock::now().time_since_epoch().count());
}

void Application::joinNetworkGame(QString host)
{
    m_networkManager.startClient(host, defaultPort);
}

void Application::createWorld(bool isNetworkGame, bool isPlayerOne, int terrainSeed)
{
    m_timer.pause(isNetworkGame);
    m_timer.adjust(0);
    m_isPlayerOne = isPlayerOne;
    m_world = std::unique_ptr<Game>(new Game(*this, isNetworkGame, isPlayerOne, terrainSeed));
    showUI();
}

void Application::endGame(bool localPlayerWins, bool showMessage)
{
    disconnectSignals();
    m_networkManager.sendGameEndedCommand(!localPlayerWins && showMessage);
    if (showMessage)
    {
        if (localPlayerWins)
        {
            QMetaObject::invokeMethod(this, "winGame", Qt::AutoConnection);
        }
        else
        {
            QMetaObject::invokeMethod(this, "loseGame", Qt::AutoConnection);
        }
    }
    else
    {
        QMetaObject::invokeMethod(this, "stopGame", Qt::AutoConnection);
    }
}

void Application::toggleUI()
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

void Application::showUI()
{
    m_isUIActive = true;
    updateQMLData();
    QString uiFile = "qrc:/source/qml/UserInterface.qml";
    QMetaObject::invokeMethod(this, "loadUI", Qt::AutoConnection, Q_ARG(QVariant, uiFile), Q_ARG(QVariant, !m_isPlayerOne));
}

void Application::hideUI()
{
    m_isUIActive = false;
    QString uiFile = "";
    QMetaObject::invokeMethod(this, "loadUI", Qt::AutoConnection, Q_ARG(QVariant, uiFile), Q_ARG(QVariant, !m_isPlayerOne));
}

Game & Application::world() const
{
    assert(m_world);
    return *m_world;
}

ActionScheduler & Application::scheduler()
{
    return m_scheduler;
}

NetworkManager & Application::networkManager()
{
    return m_networkManager;
}

Timer & Application::timer()
{
    return m_timer;
}

void Application::buttonInput(int type)
{
    if(m_world)
    {
        m_eventHandler.buttonInput(type);;
    }
}

void Application::keyInput(Qt::Key key)
{
    if(m_world)
    {
        m_eventHandler.keyInput(key);
    }
}

void Application::moveInput(int type, qreal x, qreal y)
{
    if(m_world)
    {
        m_eventHandler.moveInput(type, x, y);
    }
}

void Application::touchInput(qreal oldx, qreal oldy, qreal x, qreal y)
{
    if(m_world)
    {
        m_eventHandler.touchInput(oldx, oldy, x, y);
    }
}

void Application::sync()
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

void Application::render()
{
    if (m_world)
    {
        m_world->render();
    }
    m_window->resetOpenGLState();
}

void Application::cleanup()
{
    qDebug("cleanup");
}

void Application::handleWindowChanged(QQuickWindow * win)
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

void Application::setPaused(bool paused)
{
    m_timer.pause(paused);
}

void Application::togglePaused()
{
    m_timer.pause();
    m_networkManager.sendPauseCommand(m_timer.isPaused());
}

void Application::connectSignals(QQuickWindow *win)
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

void Application::disconnectSignals()
{
    if(m_window)
    {
        disconnect(m_window, SIGNAL(beforeRendering()), this, SLOT(render()));
        disconnect(m_window, SIGNAL(beforeSynchronizing()), this, SLOT(sync()));
        disconnect(m_window, SIGNAL(sceneGraphInvalidated()), this, SLOT(cleanup()));
    }
}

/*!
 * \brief Creates Data for QML. This is incredibly inefficient since QVariants cannot be edited only set.
 */
void Application::updateQMLData()
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

QVariant & Application::qmlData()
{
    return m_qmlData;
}

}
