#include "userinterface.h"

#include <memory>
#include <functional>

#include <QDebug>
#include <QQuickView>

#include "game.h"
#include "eventhandler.h"
#include "wagons/weaponwagon.h"

namespace terminus
{

UserInterface::UserInterface()
    : m_eventHandler(std::unique_ptr<EventHandler>(new EventHandler))
    , m_lockedWagonIndex(0)
    , m_currentWagon(nullptr)
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
}

UserInterface::~UserInterface()
{
}

void UserInterface::setGame(Game *game)
{
    m_eventHandler->setGame(game);
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

void UserInterface::switchWagonLeft()
{
    m_eventHandler->switchWagonLeft();
}

void UserInterface::switchWagonRight()
{
    m_eventHandler->switchWagonRight();
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

void UserInterface::sync()
{
    m_lockedWagonIndex = m_eventHandler->lockedWagonIndex();
    m_game = m_eventHandler->game();
    m_currentWagon = m_game->playerTrain()->wagonAt(m_lockedWagonIndex);
    auto wagon = dynamic_cast<WeaponWagon*>(m_currentWagon);
    if(wagon != nullptr)
    {
        m_wagonStatus = wagon->reloadTime();
        if(m_wagonStatus != 0)
        {

        }
        else
        {
            m_wagonStatus = wagon->chargeTime();
        }
    }
}

void UserInterface::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);
    }
}

}
