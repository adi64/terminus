#include "projectilehitcommand.h"

#include <game.h>
#include <world/world.h>
#include <world/drawables/train/train.h>
#include <world/drawables/explosion.h>
#include <player/localplayer.h>

namespace terminus
{

ProjectileHitCommand::ProjectileHitCommand(Timer::TimerMSec timeStamp, bool rightTrain, int wagonIndex, float damage)
: AbstractCommand(timeStamp)
, m_rightTrain(rightTrain)
, m_wagonIndex(wagonIndex)
, m_damage(damage)
{

}

ProjectileHitCommand::ProjectileHitCommand(Timer::TimerMSec timeStamp, QJsonObject jsonObject)
: AbstractCommand(timeStamp)
{
    m_rightTrain = jsonObject.value("rightTrain").toBool();
    m_wagonIndex = jsonObject.value("wagonIndex").toInt();
    m_damage = static_cast<float>(jsonObject.value("damage").toDouble());
}

QJsonObject ProjectileHitCommand::toJson() const
{
    QJsonObject jsonObject;
    jsonObject.insert("rightTrain", m_rightTrain);
    jsonObject.insert("wagonIndex", m_wagonIndex);
    jsonObject.insert("damage", static_cast<double>(m_damage));
    return jsonObject;
}

void ProjectileHitCommand::doWork()
{
    auto wagon = m_game->world().localPlayer().train()->wagonAt(wagonIndex());
    assert(wagon != nullptr);
    wagon->setHealth(wagon->currentHealth() - m_damage);

    m_game->world().addObject(new Explosion(m_game->world(), wagon->position()));
}

} // namespace terminus
