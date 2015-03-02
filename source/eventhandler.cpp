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
{

}

void EventHandler::buttonInput(int type)
{
    switch(type)
    {
    case NEXT_WAGON_BUTTON:
        switchToNextWagon(); break;
    case PREV_WAGON_BUTTON:
        switchToPreviousWagon(); break;
    case ACTION_BUTTON:
        touchFire(); break;
    case LEFT_MOUSE_BUTTON:
        touchFire(); break;
    default: break;
    }
}

void EventHandler::keyInput(Qt::Key key)
{
    keyPressEvent(key);
}

void EventHandler::moveInput(int type, qreal x, qreal y)
{
    switch(type)
    {
    case MOUSE_MOVEMENT:
        mouseMoveEvent(x, y); break;
    /*case TOUCH_MOVEMENT:
        touchMoveEvent(x, y); break;*/
    case GYRO_MOVEMENT:
        gyroMoveEvent(x, y); break;
    default: break;
    }
}

void EventHandler::keyPressEvent(Qt::Key key)
{
    AbstractPlayer & player = m_game->world().localPlayer();
    Camera & camera = player.camera();
    auto movement = camera.movement();

    switch(key)
    {
    case Qt::Key_W:
        if(!camera.isLocked())
        {
            movement.setZ(-1.0);
            camera.setMovement(movement);
        }
        break;
    case Qt::Key_S:
        if(!camera.isLocked())
        {
            movement.setZ(1.0);
            camera.setMovement(movement);
        }
        break;
    case Qt::Key_A:
        if(camera.isLocked())
        {
            player.switchToNextWagon();
        }
        else
        {
            movement.setX(-1.0);
            camera.setMovement(movement);
        }
        break;
    case Qt::Key_D:
        if(camera.isLocked())
        {
            player.switchToPreviousWagon();
        }
        else
        {
            movement.setX(1.0);
            camera.setMovement(movement);
        }
        break;
    case Qt::Key_Escape:
        QApplication::quit();
        break;
    case Qt::Key_Space:
        camera.toggleLocked();
        break;
    case Qt::Key_I:
        player.primaryActionDebug();
        break;
    case Qt::Key_P:
        m_game->togglePaused();
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

void EventHandler::touchFire()
{
    m_game->world().localPlayer().primaryAction();
}

}
