#include "projectilefiredcommand.h"

#include <QDebug>

#include <game.h>
#include <world/drawables/projectile.h>

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
        auto projectile = new Projectile(m_game->world());
        projectile->setSpawnedLocally(false);
        projectile->moveTo(m_startPosition);
        projectile->applyForce(m_movement);
        m_game->world().addNode(projectile);
    }

    Commands ProjectileFiredCommand::commandType() const
    {
        return Command_ProjectileFired;
    }

}
