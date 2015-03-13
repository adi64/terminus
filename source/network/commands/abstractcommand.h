#pragma once

#include <memory>

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>

#include <util/timer.h>

#include "commands.h"

namespace terminus
{
    class Game;
    class NetworkServer;
	class NetworkConnection;

    /*!
     * \brief The AbstractCommand class defines the interface that every
     * command has to implement.
     *
     * It also provides some convenience and commonly used methods and members.
     */
    class AbstractCommand
	{
	public:
        static QJsonObject VectorToJsonObject(QVector3D vector);
        static QVector3D VectorFromJsonObject(QJsonObject jsonObject);
        static QJsonValue TimeStampToJsonValue(Timer::TimerMSec timeStamp);
        static Timer::TimerMSec TimeStampFromJsonValue(QJsonValue jsonValue);

        AbstractCommand(Timer::TimerMSec timeStamp);
        virtual ~AbstractCommand();

        void setGame(Game* game);
        void run();
        virtual void doWork() = 0;
        virtual Commands commandType() const = 0;

        /*!
         * \brief Every command must define how its members can be serialized.
         * \return A QJsonObject containing the parameters / members
         */
        virtual QJsonObject toJson() const = 0;
        Timer::TimerMSec timeStamp() const;
	signals:
		void started();
		void done();
	protected:
        Timer::TimerMSec m_timeStamp;
        Game *m_game;
	};
}
