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

#include <network/networkmanager.h>

#include <game.h>

namespace terminus
{

EventHandler::EventHandler(Game * game)
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
    case GYRO_MOVEMENT:
        gyroMoveEvent(x, y); break;
    default: break;
    }
}

void EventHandler::touchInput(qreal oldx, qreal oldy, qreal x, qreal y)
{
    const double sensitivity = 0.5;

    auto offset = QVector2D(oldx, oldy) - QVector2D(x, y);
    auto rotation = offset * sensitivity;

    rotation *= QVector2D(1.0, 1.0);

    m_game->world().localPlayer().camera().rotateEvent(rotation);
}

void EventHandler::keyPressEvent(Qt::Key key)
{
    AbstractPlayer & player = m_game->world().localPlayer();

    float moveQuantum = 1.f;
    switch(key)
    {
    case Qt::Key_W:
        player.camera().moveEvent(QVector3D(0.f, 0.f, -moveQuantum));
        break;
    case Qt::Key_S:
        player.camera().moveEvent(QVector3D(0.f, 0.f, moveQuantum));
        break;
    case Qt::Key_A:
        if(player.camera().isBound())
        {
            player.switchToNextWagon();
        }
        else
        {
            player.camera().moveEvent(QVector3D(-moveQuantum, 0.f, 0.f));
        }
        break;
    case Qt::Key_D:
        if(player.camera().isBound())
        {
            player.switchToPreviousWagon();
        }
        else
        {
            player.camera().moveEvent(QVector3D(moveQuantum, 0.f, 0.f));
        }
        break;
    case Qt::Key_Escape:
        QApplication::quit();
        break;
    case Qt::Key_Space:
        player.toggleCameraLock();
        break;
    case Qt::Key_I:
        player.primaryActionDebug();
        break;
    case Qt::Key_P:
        m_game->togglePaused();
        m_game->networkManager().sendPauseCommand(m_game->isPaused());
        break;
    case Qt::Key_U:
        m_game->toggleUI();
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

    if(rotation.x() == 0 && rotation.y() == 0)
    {
        return;
    }

    // invert X and Y
    rotation *= QVector2D(-1.0, -1.0);

    m_game->world().localPlayer().camera().rotateEvent(rotation);

    QPoint globalPosition = m_game->window()->mapToGlobal(QPoint(m_game->window()->width() / 2, m_game->window()->height() / 2));
    QCursor::setPos(globalPosition);
}

void EventHandler::gyroMoveEvent(qreal x, qreal y)
{
    const double sensitivity = 0.06;

    auto offset = QVector2D(x, y);
    auto rotation = offset * sensitivity;

    m_game->world().localPlayer().camera().rotateEvent(rotation);
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
