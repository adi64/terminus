#pragma once

#include <QJsonObject>
#include <QObject>

#include <network/commands/commands.h>
#include <util/timer.h>

namespace terminus
{

class Application;
class NetworkServer;
class NetworkConnection;

/*!
 * \brief The AbstractCommand class defines the interface that every
 * command has to implement.
 *
 * It also provides some convenience methods for JSON conversion
 * and members commonly used by all commands.
 */
class AbstractCommand
{
protected:
    static QJsonObject VectorToJsonObject(QVector3D vector);
    static QVector3D VectorFromJsonObject(QJsonObject jsonObject);

public:
    AbstractCommand(Timer::TimerMSec timeStamp);
    virtual ~AbstractCommand();

    /*!
     * \brief set the game instance this command should operate on
     * \param game - ownership not taken
     */
    void setGame(Application * game);

    /*!
     * \brief get the time as counted by the sending game that this command was created and sent
     * \return timestamp in milliseconds
     */
    Timer::TimerMSec timeStamp() const;

    /*!
     * \brief Every Command must specify its type
     * \return enum constant from Commands
     *
     * \sa Commands
     */
    virtual Commands commandType() const = 0;

    /*!
     * \brief Every command must define how its members can be serialized.
     * \return A QJsonObject containing the parameters / members
     */
    virtual QJsonObject toJson() const = 0;

    /*!
     * \brief Every Commands must define its effects on the receiving game
     *
     * This Method is guaranteed to be executed in the receiving game's frame update context
     * with a valid Game instance in m_game
     */
    virtual void doWork() = 0;

protected:
    Application * m_game;
    Timer::TimerMSec m_timeStamp;
};

}
