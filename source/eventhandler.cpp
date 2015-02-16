#include "eventhandler.h"

#include <QVector3D>
#include <QApplication>
#include <QQuickWindow>

#include "game.h"
#include <world/camera.h>
#include <world/scene.h>
#include <world/drawables/train/train.h>
#include <world/drawables/train/wagons/weaponwagon.h>
#include <player/abstractplayer.h>


namespace terminus
{

EventHandler::EventHandler()
    : m_game(nullptr)
    , m_flicked(false)
    , m_flickResetted(false)
{

}

void EventHandler::setGame(Game *game)
{
    m_game = game;
}

void EventHandler::keyPressEvent(Qt::Key key)
{
    auto movement = m_game->scene()->camera().movement();

    switch(key)
    {
    case Qt::Key_W:
        movement.setZ(-1.0);
        m_game->localPlayer()->camera()->setMovement(movement);
        break;
    case Qt::Key_S:
        movement.setZ(1.0);
        m_game->localPlayer()->camera()->setMovement(movement);
        break;
    case Qt::Key_A:
        movement.setX(-1.0);
        m_game->localPlayer()->camera()->setMovement(movement);
        break;
    case Qt::Key_D:
        movement.setX(1.0);
        m_game->localPlayer()->camera()->setMovement(movement);
        break;
    case Qt::Key_Q:
        QApplication::quit();
        break;
    case Qt::Key_Space:
        m_game->localPlayer()->camera()->toggleLocked();
        break;
    case Qt::Key_Escape:
        QApplication::quit();
        break;
    case Qt::Key_I:
        m_game->localPlayer()->primaryActionDebug();
        break;
    case Qt::Key_O:
        m_game->localPlayer()->primaryAction();
        break;
    case Qt::Key_P:
        m_game->togglePaused();
        break;
    case Qt::Key_Plus:
        m_game->localPlayer()->switchToNextWagon();
        break;
    case Qt::Key_Minus:
        m_game->localPlayer()->switchToPreviousWagon();
        break;
    default:
        break;
    }
}

void EventHandler::keyReleaseEvent(Qt::Key key)
{
    auto movement = m_game->scene()->camera().movement();

    switch(key)
    {
    case Qt::Key_W:
        movement.setZ(0.0);
        m_game->localPlayer()->camera()->setMovement(movement);
        break;
    case Qt::Key_S:
        movement.setZ(0.0);
        m_game->localPlayer()->camera()->setMovement(movement);
        break;
    case Qt::Key_A:
        movement.setX(0.0);
        m_game->localPlayer()->camera()->setMovement(movement);
        break;
    case Qt::Key_D:
        movement.setX(0.0);
        m_game->localPlayer()->camera()->setMovement(movement);
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

    m_game->localPlayer()->camera()->setRotation(rotation);

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

    m_game->localPlayer()->camera()->setRotation(rotation);
}

void EventHandler::gyroMoveEvent(qreal x, qreal y)
{
    const double sensitivity = 0.005;

    auto offset = QVector2D(x, y);
    auto rotation = offset * sensitivity;

    m_game->localPlayer()->camera()->setRotation(rotation);
}

void EventHandler::switchToNextWagon()
{
    m_game->localPlayer()->switchToNextWagon();
}

void EventHandler::switchToPreviousWagon()
{
    m_game->localPlayer()->switchToPreviousWagon();
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

    m_game->localPlayer()->camera()->setMovement(QVector3D(direction, 0.0f, 0.0f));

    if(direction > 0)
    {
        if(m_game->localPlayer()->camera()->isLocked() && ((m_game->localPlayer()->selectedWagonIndex() + 1) < m_game->playerTrain()->size()))
        {
            m_flickDirection = direction;
            m_flicked = (distance > threshold);
        }
    }
    if(direction < 0)
    {
        if(m_game->localPlayer()->camera()->isLocked() && m_game->localPlayer()->selectedWagonIndex() > 0)
        {
            m_flickDirection = direction;
            m_flicked = (distance > threshold);
        }
    }
}

void EventHandler::flickReset()
{
    m_game->localPlayer()->camera()->setMovement(QVector3D(0.f, 0.f, 0.f));
    m_flickResetted = true;

    if(m_flicked)
    {
        if(m_flickDirection > 0)
        {
            m_game->localPlayer()->switchToNextWagon();
        }
        if(m_flickDirection < 0)
        {
            m_game->localPlayer()->switchToPreviousWagon();
        }
        m_flicked = false;
    }
}*/

void EventHandler::touchChargeFire()
{
    auto wagon = dynamic_cast<WeaponWagon*>(m_game->playerTrain()->wagonAt(m_game->localPlayer()->selectedWagonIndex()));
    if(wagon != nullptr)
    {
        wagon->setChargeProjectile(true);
    }
}

void EventHandler::touchFire()
{
    m_game->localPlayer()->primaryAction();
}

}
