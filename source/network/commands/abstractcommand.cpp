#include "abstractcommand.h"

#include <assert.h>

#include <QVector3D>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>

#include <game.h>

namespace terminus
{

QJsonObject AbstractCommand::VectorToJsonObject(QVector3D vector)
{
    QJsonObject jsonObject;
    jsonObject.insert("x", vector.x());
    jsonObject.insert("y", vector.x());
    jsonObject.insert("z", vector.x());

    return jsonObject;
}

QVector3D AbstractCommand::VectorFromJsonObject(QJsonObject jsonObject)
{
    auto x = jsonObject.value("x");
    auto y = jsonObject.value("y");
    auto z = jsonObject.value("z");

    assert(!x.isUndefined());
    assert(!y.isUndefined());
    assert(!z.isUndefined());

    return QVector3D(x.toDouble(), y.toDouble(), z.toDouble());
}

QJsonValue AbstractCommand::TimeStampToJsonValue(Timer::TimerMSec timeStamp)
{
    auto jsonValue = QJsonValue(static_cast<double>(timeStamp));
    return jsonValue;
}

Timer::TimerMSec AbstractCommand::TimeStampFromJsonValue(QJsonValue jsonValue)
{
    assert(jsonValue.isDouble());
    auto timeStamp = static_cast<Timer::TimerMSec>(jsonValue.toDouble());
    return timeStamp;
}

AbstractCommand::AbstractCommand()
: AbstractCommand(m_game->timer().get())
{

}

AbstractCommand::AbstractCommand(Timer::TimerMSec timeStamp)
: m_timeStamp(timeStamp)
, m_game(nullptr)
{

}

AbstractCommand::~AbstractCommand()
{

}

void AbstractCommand::setGame(Game *game)
{
    m_game = game;
}

Timer::TimerMSec AbstractCommand::timeStamp() const
{
    return m_timeStamp;
}

void AbstractCommand::run() {
    doWork();
}

} // namespace terminus
