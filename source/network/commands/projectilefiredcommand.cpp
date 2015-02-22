#include "projectilefiredcommand.h"

#include <QDebug>
namespace terminus
{
    ProjectileFiredCommand::ProjectileFiredCommand(TimeStamp timeStamp, QVector3D startPosition, QVector3D movement)
        : AbstractCommand(timeStamp)
        , m_startPosition(startPosition)
        , m_movement(movement)
	{

    }

    ProjectileFiredCommand::ProjectileFiredCommand(TimeStamp timeStamp, QJsonObject jsonObject)
        : AbstractCommand(timeStamp)
    {
        m_startPosition = VectorFromJsonObject(jsonObject.value("startPosition").toObject());
        m_movement = VectorFromJsonObject(jsonObject.value("movement").toObject());
    }

    QJsonObject ProjectileFiredCommand::toJson() const
    {
        QJsonObject jsonObject;
        jsonObject.insert("startPosition", VectorToJsonObject(m_startPosition));
        jsonObject.insert("movement", VectorToJsonObject(m_movement));
        return jsonObject;
    }

    void ProjectileFiredCommand::doWork()
    {
        // insert projectile into game world
    }

    Commands ProjectileFiredCommand::commandType() const
    {
        return Command_ProjectileFired;
    }

}
