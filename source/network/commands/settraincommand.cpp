#include "settraincommand.h"

#include <QJsonArray>

#include <game.h>
#include <world/world.h>
#include <world/drawables/train/train.h>

namespace terminus
{

SetTrainCommand::SetTrainCommand(Timer::TimerMSec timeStamp, const Train & train)
    : AbstractCommand(timeStamp)
{
    m_trainConfig.clear();
    for(unsigned int i=0; i<train.size(); i++)
    {
        m_trainConfig.append(static_cast<int>(train.wagonAt(i)->wagonType()));
    }
}

SetTrainCommand::SetTrainCommand(Timer::TimerMSec timeStamp, QJsonObject jsonObject)
    : AbstractCommand(timeStamp)
{
    QJsonArray playerTrainConfigArray = jsonObject.value("trainConfig").toArray();
    for(int i = 0; i < playerTrainConfigArray.size(); i++)
    {
        m_trainConfig.append(playerTrainConfigArray.at(i).toInt());
    }
}

QJsonObject SetTrainCommand::toJson() const
{
    QJsonObject jsonObject;

    QVariantList variantList;
    for(int wagonTypeInt : m_trainConfig)
    {
        variantList.append(QVariant(wagonTypeInt));
    }

    QJsonArray playerTrainConfigArray = QJsonArray::fromVariantList(variantList);

    jsonObject.insert("trainConfig", playerTrainConfigArray);

    return jsonObject;
}

void SetTrainCommand::doWork()
{
    // clear enemy train
    while(m_game->world().enemyPlayerTrain().size() > 1)
    {
        m_game->world().enemyPlayerTrain().removeWagon(m_game->world().enemyPlayerTrain().size() - 1);
    }

    // add wagons
    for(const int wagonTypeInt : m_trainConfig)
    {
        WagonType wagonType = static_cast<WagonType>(wagonTypeInt);

        if(wagonType != WagonType::ENGINE_WAGON)
            m_game->world().enemyPlayerTrain().addWagon(wagonType);
    }

    if(m_game->networkManager().mode() == NetworkManager::Mode::MultiplayerHost)
    {
        // we are host and received the client's train config
        // this means the client is ready to receive our train config
        m_game->networkManager().sendSetTrainCommand();
    }
    else if(m_game->networkManager().mode() == NetworkManager::Mode::MultiplayerClient)
    {
        // we already sent our train config and now received the host's train config
        // this means that we are ready and wait for host's go
        m_game->networkManager().sendClientReadyCommand();
    }

    m_game->showUI();
}

Commands SetTrainCommand::commandType() const
{
    return Command_SetTrain;
}

const QList<int> &SetTrainCommand::trainConfig() const
{
    return m_trainConfig;
}

} //namespace terminus
