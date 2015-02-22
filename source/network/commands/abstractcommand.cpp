#include "abstractcommand.h"

#include <assert.h>

#include <QVector3D>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>

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

QJsonValue AbstractCommand::TimeStampToJsonValue(TimeStamp timeStamp)
{
    auto jsonValue = QJsonValue(static_cast<double>(timeStamp.count()));
    return jsonValue;
}

TimeStamp AbstractCommand::TimeStampFromJsonValue(QJsonValue jsonValue)
{
    assert(jsonValue.isDouble());
    auto timeStamp = TimeStamp(static_cast<long>(jsonValue.toDouble()));
    return timeStamp;
}

AbstractCommand::AbstractCommand(TimeStamp timeStamp)
    : m_timeStamp(timeStamp)
{

}

NetworkConnection* AbstractCommand::clientConnection() {
    return m_clientConnection;
}

TimeStamp AbstractCommand::timeStamp() const
{
    return m_timeStamp;
}

void AbstractCommand::run() {
    doWork();
}

} // namespace terminus
