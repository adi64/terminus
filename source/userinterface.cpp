#include "userinterface.h"

#include <memory>
#include <functional>
#include <assert.h>

#include <QDebug>
#include <QQuickView>

#include <player/abstractplayer.h>
#include <world/drawables/train/qmltrain.h>
#include <world/drawables/train/wagons/qmlwagon.h>

#include "game.h"
#include "eventhandler.h"

namespace terminus
{

UserInterface::UserInterface(Game *game)
    : m_game(game)
    , m_eventHandler(std::unique_ptr<EventHandler>(new EventHandler(game)))
    , m_currentWagonIndex(0)
    , m_playerQMLTrain(m_game->playerTrain()->qmlTrain())
    , m_enemyQMLTrain(m_game->enemyTrain()->qmlTrain())
{
    assert(m_game != nullptr);
    connect(m_game, SIGNAL(gameSyncCompleted()), this, SLOT(sync()), Qt::DirectConnection);
}

UserInterface::~UserInterface()
{
}

void UserInterface::keyPressEvent(Qt::Key key)
{
    m_eventHandler->keyPressEvent(key);
}

void UserInterface::keyReleaseEvent(Qt::Key key)
{
    m_eventHandler->keyReleaseEvent(key);
}

void UserInterface::mouseMoveEvent(qreal x, qreal y)
{
    m_eventHandler->mouseMoveEvent(x, y);
}

void UserInterface::touchMoveEvent(qreal x, qreal y)
{
    m_eventHandler->touchMoveEvent(x, y);
}

void UserInterface::gyroMoveEvent(qreal x, qreal y)
{
    m_eventHandler->gyroMoveEvent(x, y);
}

void UserInterface::switchToNextWagon()
{
    m_eventHandler->switchToNextWagon();
}

void UserInterface::switchToPreviousWagon()
{
    m_eventHandler->switchToPreviousWagon();
}

/*void UserInterface::flickEvent(qreal startX, qreal x)
{
    m_eventHandler->flickEvent(startX, x);
}

void UserInterface::flickReset()
{
    m_eventHandler->flickReset();
}*/

void UserInterface::touchChargeFire()
{
    m_eventHandler->touchChargeFire();
}

void UserInterface::touchFire()
{
    m_eventHandler->touchFire();
}

QMLTrain *UserInterface::playerQMLTrain()
{
    return m_playerQMLTrain;
}

QMLTrain *UserInterface::enemyQMLTrain()
{
    return m_enemyQMLTrain;
}

int UserInterface::currentWagonIndex()
{
    return m_currentWagonIndex;
}

void UserInterface::sync()
{
    m_currentWagonIndex = m_game->localPlayer()->selectedWagonIndex();
}
}
