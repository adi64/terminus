#include "projectilefiredcommand.h"

#include <QDebug>

#include <game.h>
#include <world/drawables/projectile.h>
#include <resources/soundmanager.h>

namespace terminus
{
    ProjectileFiredCommand::ProjectileFiredCommand(Timer::TimerMSec timeStamp, QVector3D startPosition, QVector3D velocity)
        : AbstractCommand(timeStamp)
        , m_startPosition(startPosition)
        , m_velocity(velocity)
	{

    }

    ProjectileFiredCommand::ProjectileFiredCommand(Timer::TimerMSec timeStamp, QJsonObject jsonObject)
        : AbstractCommand(timeStamp)
    {
        m_startPosition = VectorFromJsonObject(jsonObject.value("startPosition").toObject());
        m_velocity = VectorFromJsonObject(jsonObject.value("velocity").toObject());
    }

    QJsonObject ProjectileFiredCommand::toJson() const
    {
        QJsonObject jsonObject;
        jsonObject.insert("startPosition", VectorToJsonObject(m_startPosition));
        jsonObject.insert("velocity", VectorToJsonObject(m_velocity));
        return jsonObject;
    }

    void ProjectileFiredCommand::doWork()
    {
        auto projectile = new Projectile(m_game->world());
        projectile->setSpawnedLocally(false);
        projectile->moveTo(m_startPosition);
        projectile->setLinearVelocity(m_velocity);

        m_game->world().addObject(projectile);

        SoundManager::getInstance()->playSound("shot");
    }

    Commands ProjectileFiredCommand::commandType() const
    {
        return Command_ProjectileFired;
    }

    QVector3D ProjectileFiredCommand::startPosition() const
    {
        return m_startPosition;
    }

    QVector3D ProjectileFiredCommand::velocity() const
    {
        return m_velocity;
    }

}
