#include "eventhandler.h"

#include <QVector3D>
#include <QApplication>
#include <QQuickWindow>

#include <world/camera.h>
#include <world/world.h>
#include <world/drawables/train/train.h>
#include <world/drawables/train/wagons/weaponwagon.h>
#include <player/abstractplayer.h>
#include <player/localplayer.h>

#include "game.h"

namespace terminus
{

EventHandler::EventHandler(Game *game)
    : m_game(game)
    , m_flicked(false)
    , m_flickResetted(false)
{

}

void EventHandler::keyPressEvent(Qt::Key key)
{
    AbstractPlayer & player = m_game->world().localPlayer();
    Camera & camera = player.camera();
    auto movement = camera.movement();

    switch(key)
    {
    case Qt::Key_W:
        movement.setZ(-1.0);
        camera.setMovement(movement);
        break;
    case Qt::Key_S:
        movement.setZ(1.0);
        camera.setMovement(movement);
        break;
    case Qt::Key_A:
        movement.setX(-1.0);
        camera.setMovement(movement);
        break;
    case Qt::Key_D:
        movement.setX(1.0);
        camera.setMovement(movement);
        break;
    case Qt::Key_Q:
        QApplication::quit();
        break;
    case Qt::Key_Space:
        camera.toggleLocked();
        break;
    case Qt::Key_Escape:
        QApplication::quit();
        break;
    case Qt::Key_I:
        player.primaryActionDebug();
        break;
    case Qt::Key_O:
        player.primaryAction();
        break;
    case Qt::Key_P:
        m_game->togglePaused();
        break;
    case Qt::Key_Plus:
        player.switchToNextWagon();
        break;
    case Qt::Key_Minus:
        player.switchToPreviousWagon();
        break;
    default:
        break;
    }
}

void EventHandler::keyReleaseEvent(Qt::Key key)
{
    Camera & camera = m_game->world().localPlayer().camera();
    auto movement = camera.movement();

    switch(key)
    {
    case Qt::Key_W:
        movement.setZ(0.0);
        camera.setMovement(movement);
        break;
    case Qt::Key_S:
        movement.setZ(0.0);
        camera.setMovement(movement);
        break;
    case Qt::Key_A:
        movement.setX(0.0);
        camera.setMovement(movement);
        break;
    case Qt::Key_D:
        movement.setX(0.0);
        camera.setMovement(movement);
        break;
    default:
        break;
    }
}

void EventHandler::mouseMoveEvent(qreal x, qreal y)
{
    const double sensitivity = 0.5;

    auto oldPosition = QVector2D(m_game->window()->width() / 2, m_game->window()->height() / 2);
    auto offset = oldPosition - QVector2D(x, y);
    auto rotation = offset * sensitivity;

    // invert X
    rotation *= QVector2D(-1.0, 1.0);

    m_game->world().localPlayer().camera().setRotation(rotation);

    QPoint globalPosition = m_game->window()->mapToGlobal(QPoint(m_game->window()->width() / 2, m_game->window()->height() / 2));
    QCursor::setPos(globalPosition);
}

void EventHandler::touchMoveEvent(qreal x, qreal y)
{
    const double sensitivity = 0.05;

    auto offset = QVector2D(x, y);
    auto rotation = offset * sensitivity;

    // invert X
    rotation *= QVector2D(-1.0, 1.0);

    m_game->world().localPlayer().camera().setRotation(rotation);
}

void EventHandler::gyroMoveEvent(qreal x, qreal y)
{
    const double sensitivity = 0.005;

    auto offset = QVector2D(x, y);
    auto rotation = offset * sensitivity;

    m_game->world().localPlayer().camera().setRotation(rotation);
}

void EventHandler::switchToNextWagon()
{
    m_game->world().localPlayer().switchToNextWagon();
}

void EventHandler::switchToPreviousWagon()
{
    m_game->world().localPlayer().switchToPreviousWagon();
}

/*void EventHandler::flickEvent(qreal startx, qreal x)
{
    if(m_flickResetted)
    {
        m_flickResetted = false;
        return;
    }

    #ifdef Q_OS_MAC
        auto width = m_game->window()->width() * 2;
    #else
        auto width = m_game->window()->width();
    #endif
    auto direction = (x - startx) / (0.2f * width);
    auto distance = abs(x - startx);

    auto threshold = width * 0.2;

    m_game->world().localPlayer().camera().setMovement(QVector3D(direction, 0.0f, 0.0f));

    if(direction > 0)
    {
        if(m_game->world().localPlayer().camera().isLocked())
        {
            m_flickDirection = direction;
            m_flicked = (distance > threshold);
        }
    }
    if(direction < 0)
    {
        if(m_game->world().localPlayer().camera().isLocked())
        {
            m_flickDirection = direction;
            m_flicked = (distance > threshold);
        }
    }
}

void EventHandler::flickReset()
{
    m_game->world().localPlayer().camera().setMovement(QVector3D(0.f, 0.f, 0.f));
    m_flickResetted = true;

    if(m_flicked)
    {
        if(m_flickDirection > 0)
        {
            m_game->world().localPlayer().switchToNextWagon();
        }
        if(m_flickDirection < 0)
        {
            m_game->world().localPlayer().switchToPreviousWagon();
        }
        m_flicked = false;
    }
}*/

void EventHandler::touchChargeFire()
{
    // TODO FIXME charge will be removed
}

void EventHandler::touchFire()
{
    m_game->world().localPlayer().primaryAction();
}

}
