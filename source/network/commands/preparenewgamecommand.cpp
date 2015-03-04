#include "preparenewgamecommand.h"

#include <QDebug>

#include <game.h>

namespace terminus
{
    PrepareNewGameCommand::PrepareNewGameCommand(bool isPlayerOne, unsigned int terrainSeed)
        : AbstractCommand()
        , m_isPlayerOne(isPlayerOne)
        , m_terrainSeed(terrainSeed)
	{

    }

    PrepareNewGameCommand::PrepareNewGameCommand(Timer::TimerMSec timeStamp, QJsonObject jsonObject)
        : AbstractCommand(timeStamp)
    {
        m_isPlayerOne = jsonObject.value("isPlayerOne").toBool();
        m_terrainSeed = static_cast<unsigned int>(jsonObject.value("terrainSeed").toInt());
    }

    QJsonObject PrepareNewGameCommand::toJson() const
    {
        QJsonObject jsonObject;
        jsonObject.insert("isPlayerOne", QJsonValue(m_isPlayerOne));
        jsonObject.insert("terrainSeed", QJsonValue(static_cast<signed int>(m_terrainSeed)));
        return jsonObject;
    }

    void PrepareNewGameCommand::doWork()
    {
        m_game->startNetworkGame(m_isPlayerOne, m_terrainSeed);
    }

    Commands PrepareNewGameCommand::commandType() const
    {
        return Command_PrepareNewGame;
    }

    bool PrepareNewGameCommand::isPlayerOne() const
    {
        return m_isPlayerOne;
    }

    unsigned int PrepareNewGameCommand::terrainSeed() const
    {
        return m_terrainSeed;
    }

}
