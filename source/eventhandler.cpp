#include "eventhandler.h"

#include <QVector3D>
#include <QApplication>
#include <QQuickWindow>

#include "game.h"
#include "camera.h"
#include "scene.h"
#include "resources/soundmanager.h"
#include "train.h"
#include "wagons/weaponwagon.h"


namespace terminus
{

EventHandler::EventHandler(Game *game)
    : m_game(game)
    , m_lockedWagonIndex(0)
    , m_flicked(false)
    , m_flickResetted(false)
{

}

void EventHandler::keyPressEvent(Qt::Key key)
{
    auto movement = m_game->scene()->camera().movement();

    switch(key)
    {
    case Qt::Key_W:
        movement.setZ(-1.0);
        m_game->scene()->camera().setMovement(movement);
        break;
    case Qt::Key_S:
        movement.setZ(1.0);
        m_game->scene()->camera().setMovement(movement);
        break;
    case Qt::Key_A:
        movement.setX(-1.0);
        m_game->scene()->camera().setMovement(movement);
        break;
    case Qt::Key_D:
        movement.setX(1.0);
        m_game->scene()->camera().setMovement(movement);
        break;
    case Qt::Key_Q:
        QApplication::quit();
        break;
    case Qt::Key_Space:
        m_game->scene()->camera().toggleLocked();
        break;
    case Qt::Key_Escape:
        QApplication::quit();
        break;
    case Qt::Key_U:
        SoundManager::getInstance()->playSound("angriff");
        break;
    case Qt::Key_I:
        SoundManager::getInstance()->playSound("shot");
        m_game->playerTrain()->wagonAt(m_lockedWagonIndex)->primaryActionDebug();
        break;
    case Qt::Key_O:
        SoundManager::getInstance()->playSound("alarm");
        break;
    case Qt::Key_M:
        SoundManager::getInstance()->toggleBackgroundMusic();
        break;
    case Qt::Key_Plus:
        if(m_game->scene()->camera().isLocked() && ((m_lockedWagonIndex + 1) < m_game->playerTrain()->size()))
        {
            m_lockedWagonIndex++;
            m_game->scene()->camera().lockToObject(m_game->playerTrain()->wagonAt(m_lockedWagonIndex));
        }
        break;
    case Qt::Key_Minus:
        if(m_game->scene()->camera().isLocked() && m_lockedWagonIndex > 0)
        {
            m_lockedWagonIndex--;
            m_game->scene()->camera().lockToObject(m_game->playerTrain()->wagonAt(m_lockedWagonIndex));
        }
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
        m_game->scene()->camera().setMovement(movement);
        break;
    case Qt::Key_S:
        movement.setZ(0.0);
        m_game->scene()->camera().setMovement(movement);
        break;
    case Qt::Key_A:
        movement.setX(0.0);
        m_game->scene()->camera().setMovement(movement);
        break;
    case Qt::Key_D:
        movement.setX(0.0);
        m_game->scene()->camera().setMovement(movement);
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

    m_game->scene()->camera().setRotation(rotation);

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

    m_game->scene()->camera().setRotation(rotation);
}

void EventHandler::gyroMoveEvent(qreal x, qreal y)
{
    const double sensitivity = 0.005;

    auto offset = QVector2D(x, y);
    auto rotation = offset * sensitivity;

    m_game->scene()->camera().setRotation(rotation);
}

void EventHandler::flickEvent(qreal startx, qreal x)
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

    m_game->scene()->camera().setMovement(QVector3D(direction, 0.0f, 0.0f));

    if(direction > 0)
    {
        if(m_game->scene()->camera().isLocked() && ((m_lockedWagonIndex + 1) < m_game->playerTrain()->size()))
        {
            m_flickDirection = direction;
            m_flicked = (distance > threshold);
        }
    }
    if(direction < 0)
    {
        if(m_game->scene()->camera().isLocked() && m_lockedWagonIndex > 0)
        {
            m_flickDirection = direction;
            m_flicked = (distance > threshold);
        }
    }
}

void EventHandler::flickReset()
{
    m_game->scene()->camera().setMovement(QVector3D(0.f, 0.f, 0.f));
    m_flickResetted = true;

    if(m_flicked)
    {
        if(m_flickDirection > 0)
        {
            m_lockedWagonIndex++;
            m_game->scene()->camera().lockToObject(m_game->playerTrain()->wagonAt(m_lockedWagonIndex));
        }
        if(m_flickDirection < 0)
        {
            m_lockedWagonIndex--;
            m_game->scene()->camera().lockToObject(m_game->playerTrain()->wagonAt(m_lockedWagonIndex));
        }
        m_flicked = false;
    }
}

void EventHandler::touchChargeFire()
{
    auto wagon = dynamic_cast<WeaponWagon*>(m_game->playerTrain()->wagonAt(m_lockedWagonIndex));
    if(wagon != nullptr)
    {
        wagon->setChargeProjectile(true);
    }
}

void EventHandler::touchFire()
{
    m_game->playerTrain()->wagonAt(m_lockedWagonIndex)->primaryAction();
    SoundManager::getInstance()->playSound("shot");
}

}
