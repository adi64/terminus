#pragma once

#include <memory>
#include <chrono>

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>

#include "commands.h"

using TimeStamp = std::chrono::duration<long,std::milli>;

namespace terminus
{
    class Game;
    class NetworkServer;
	class NetworkConnection;

    class AbstractCommand
	{
	public:
        static QJsonObject VectorToJsonObject(QVector3D vector);
        static QVector3D VectorFromJsonObject(QJsonObject jsonObject);
        static QJsonValue TimeStampToJsonValue(TimeStamp timeStamp);
        static TimeStamp TimeStampFromJsonValue(QJsonValue jsonValue);

        AbstractCommand(TimeStamp timeStamp);
        virtual ~AbstractCommand();

        void setGame(Game* game);
        void run();
        virtual void doWork() = 0;
        virtual Commands commandType() const = 0;
        virtual QJsonObject toJson() const = 0;
        TimeStamp timeStamp() const;
	signals:
		void started();
		void done();
	protected:
        TimeStamp m_timeStamp;
        Game *m_game;
	};
}
