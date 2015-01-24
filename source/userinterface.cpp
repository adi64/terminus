#include "userinterface.h"

#include <memory>
#include <functional>

#include <QDebug>

#include "game.h"
#include "eventhandler.h"

namespace terminus
{

UserInterface::UserInterface()
    : m_eventHandler(std::unique_ptr<EventHandler>(new EventHandler))
{
}

UserInterface::~UserInterface()
{
}

void UserInterface::keyPressEvent(Qt::Key key, Game *game)
{
    m_eventHandler->keyPressEvent(key, game);
}

void UserInterface::keyReleaseEvent(Qt::Key key, Game *game)
{
    m_eventHandler->keyReleaseEvent(key, game);
}

void UserInterface::mouseMoveEvent(qreal x, qreal y, Game *game)
{
    m_eventHandler->mouseMoveEvent(x, y, game);
}

void UserInterface::touchMoveEvent(qreal x, qreal y, Game *game)
{
    m_eventHandler->touchMoveEvent(x, y, game);
}

void UserInterface::gyroMoveEvent(qreal x, qreal y, Game *game)
{
    m_eventHandler->gyroMoveEvent(x, y, game);
}

void UserInterface::flickEvent(qreal startX, qreal x, Game *game)
{
    m_eventHandler->flickEvent(startX, x, game);
}

void UserInterface::flickReset(Game *game)
{
    m_eventHandler->flickReset(game);
}

void UserInterface::touchChargeFire(Game *game)
{
    m_eventHandler->touchChargeFire(game);
}

void UserInterface::touchFire(Game *game)
{
    m_eventHandler->touchFire(game);
}

}
