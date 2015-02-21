#pragma once

#include <QObject>
#include <QString>

namespace terminus {

class AbstractCommand;

class NetworkEndpoint : public QObject
{
    Q_OBJECT

public:
    void handleNewCommands();

protected:
    AbstractCommand *createCommandForRequest(const QString &request);
};

}
