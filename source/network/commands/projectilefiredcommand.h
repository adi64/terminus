#pragma once

#include <QVector3D>

#include <network/commands/abstractcommand.h>

namespace terminus
{

/*!
 * \brief The ProjectileFiredCommand indicates that a projectile has been fired
 * and contains the position as well as the movement vector.
 *
 * A projectile that gets spawned as result of this command has the
 * AbstractPhysicsObject::m_spawnedLocally flag set to false. This means that
 * collisions of this projectile do not deal any damage.
 *
 * \sa ProjectileHitCommand, Projectile
 */
class ProjectileFiredCommand : public AbstractCommand
{
public:
    ProjectileFiredCommand(Timer::TimerMSec timeStamp, QVector3D startPosition, QVector3D velocity);
    ProjectileFiredCommand(Timer::TimerMSec timeStamp, QJsonObject jsonObject);

    virtual QJsonObject toJson() const override;
    virtual void doWork() override;

    virtual Commands commandType() const override;

    QVector3D startPosition() const;
    QVector3D velocity() const;

protected:

    QVector3D m_startPosition;
    QVector3D m_velocity;
};

}
