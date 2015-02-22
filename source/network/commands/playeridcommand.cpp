#include "playeridcommand.h"

#include <QDebug>
namespace terminus
{
    PlayerIdCommand::PlayerIdCommand(TimeStamp timeStamp, int ID)
        : AbstractCommand(timeStamp)
        , m_ID(ID)
	{

    }

    PlayerIdCommand::PlayerIdCommand(TimeStamp timeStamp, QJsonObject jsonObject)
        : AbstractCommand(timeStamp)
    {
        m_ID = jsonObject.value("ID").toInt();
    }

    QJsonObject PlayerIdCommand::toJson() const
    {
        QJsonObject jsonObject;
        jsonObject.insert("ID", QJsonValue(m_ID));
        return jsonObject;
    }

    void PlayerIdCommand::doWork()
    {
        // set player ID
    }

    Commands PlayerIdCommand::commandType() const
    {
        return Command_PlayerID;
    }

}
