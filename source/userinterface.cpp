#include "userinterface.h"

#include <memory>
#include <functional>

#include <QDebug>
#include <QQuickView>

#include <player/abstractplayer.h>
#include <world/drawables/train/wagons/weaponwagon.h>
#include <world/drawables/train/wagons/enginewagon.h>
#include <world/drawables/train/wagons/repairwagon.h>

#include "game.h"
#include "eventhandler.h"

namespace terminus
{

UserInterface::UserInterface()
    : m_eventHandler(std::unique_ptr<EventHandler>(new EventHandler))
    , m_lockedWagonIndex(0)
    , m_currentWagon(nullptr)
    , m_charge(0.f)
    , m_reload(0.f)
{

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

float UserInterface::charge() const
{
    return m_charge;
}

float UserInterface::reload() const
{
    return m_reload;
}

QString UserInterface::wagonType() const
{
    return m_wagonType;
}

void UserInterface::sync(Game *game)
{
    m_eventHandler->setGame(game);
    auto oldWagon = m_currentWagon;
    m_currentWagon = game->playerTrain()->wagonAt(game->localPlayer()->selectedWagonIndex());

    auto wagon = dynamic_cast<WeaponWagon*>(m_currentWagon);
    if(wagon != nullptr)
    {
        m_wagonType = "WeaponWagon";
        wagonTypeChanged();

        auto reload = static_cast<float>(wagon->reloadTime());
        auto charge = static_cast<float>(wagon->chargeTime());

        if(charge > 0.f || oldWagon != m_currentWagon)
        {
            m_charge = (charge / 3000.f);
            chargeChanged();
        }
        if(reload > 0.f || oldWagon != m_currentWagon)
        {
            m_reload = (reload / 5000.f);
            reloadChanged();
        }
        return;
    }

    auto wagon2 = dynamic_cast<RepairWagon*>(m_currentWagon);
    if(wagon2 != nullptr)
    {
        m_wagonType = "RepairWagon";
        m_charge = 0;
        m_reload = 0;
        wagonTypeChanged();
        chargeChanged();
        reloadChanged();
        return;
    }

    auto wagon3 = dynamic_cast<EngineWagon*>(m_currentWagon);
    if(wagon3 != nullptr)
    {
        m_wagonType = "EngineWagon";
        m_charge = 0;
        m_reload = 0;
        wagonTypeChanged();
        chargeChanged();
        reloadChanged();
        return;
    }
}
}
