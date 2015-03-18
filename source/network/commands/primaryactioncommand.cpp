#include "primaryactioncommand.h"

#include <game.h>
#include <player/abstractplayer.h>
#include <world/camera.h>
#include <world/drawables/train/train.h>
#include <world/drawables/train/wagons/abstractwagon.h>

namespace terminus
{

PrimaryActionCommand::PrimaryActionCommand(Timer::TimerMSec timeStamp, unsigned int selectedWagonIndex, QVector3D aimDirection)
    : AbstractCommand(timeStamp)
    , m_selectedWagonIndex(selectedWagonIndex)
    , m_aimDirection(aimDirection)
{

}

PrimaryActionCommand::PrimaryActionCommand(Timer::TimerMSec timeStamp, QJsonObject jsonObject)
    : AbstractCommand(timeStamp)
{
    m_selectedWagonIndex = static_cast<unsigned int>(jsonObject.value("selectedWagonIndex").toInt());
    m_aimDirection = VectorFromJsonObject(jsonObject.value("aimDirection").toObject());
}

QJsonObject PrimaryActionCommand::toJson() const
{
    QJsonObject jsonObject;
    jsonObject.insert("selectedWagonIndex", static_cast<signed int>(m_selectedWagonIndex));
    jsonObject.insert("aimDirection", VectorToJsonObject(m_aimDirection));
    return jsonObject;
}

void PrimaryActionCommand::doWork()
{
    // TODO FIXME update camera aim vector
    //m_game->world().enemyPlayer().

    m_game->world().enemyPlayer().switchToWagon(m_selectedWagonIndex);

    m_game->world().enemyPlayer().primaryActionDebug();

    // TODO FIXME set spawnedLocally to spawned projectile, if any
}

}
