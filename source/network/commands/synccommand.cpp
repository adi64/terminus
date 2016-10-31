#include "synccommand.h"

#include <cassert>

#include <QJsonArray>

#include <application.h>
#include <world/drawables/train/train.h>
#include <world/drawables/train/wagons/abstractwagon.h>
#include <world/game.h>

namespace terminus
{

SyncCommand::SyncCommand(Timer::TimerMSec timeStamp, const Train & playerTrain)
: AbstractCommand(timeStamp)
{
    m_wagonHealthVector.clear();

    for(unsigned int i=0; i<playerTrain.size(); ++i)
    {
        m_wagonHealthVector.push_back(playerTrain.wagonAt(i)->currentHealth());
    }

    m_velocity = playerTrain.velocity();
    m_travelledDistance = playerTrain.travelledDistance();
}

SyncCommand::SyncCommand(Timer::TimerMSec timeStamp, QJsonObject jsonObject)
: AbstractCommand(timeStamp)
{
    auto jsonArray = jsonObject.value("wagonHealthVector").toArray();

    for(QJsonValue health : jsonArray)
    {
        m_wagonHealthVector.push_back(static_cast<float>(health.toDouble()));
    }

    m_velocity = static_cast<float>(jsonObject.value("velocity").toDouble());
    m_travelledDistance = static_cast<float>(jsonObject.value("travelledDistance").toDouble());
}

QJsonObject SyncCommand::toJson() const
{
    QJsonObject jsonObject;

    QJsonArray jsonArray;

    for(auto health : m_wagonHealthVector)
    {
        jsonArray.append(health);
    }

    jsonObject.insert("wagonHealthVector", jsonArray);
    jsonObject.insert("velocity", static_cast<double>(m_velocity));
    jsonObject.insert("travelledDistance", static_cast<double>(m_travelledDistance));

    return jsonObject;
}

void SyncCommand::doWork()
{
    assert(m_game->world().enemyPlayerTrain().size() == m_wagonHealthVector.size());

    // synchronize timer if we are client
    if(m_game->networkManager().mode() == NetworkManager::Mode::MultiplayerClient)
    {
        m_game->timer().adjust(m_timeStamp);
    }

    for(unsigned int i=0; i<m_wagonHealthVector.size(); ++i)
    {
        m_game->world().enemyPlayerTrain().wagonAt(i)->setHealth(m_wagonHealthVector[i]);
    }

    m_game->world().enemyPlayerTrain().setVelocity(m_velocity);
    m_game->world().enemyPlayerTrain().setTravelledDistance(m_travelledDistance);
}

} // namespace terminus
