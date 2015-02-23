#pragma once

#include <memory>

#include <QObject>

#include <world/scene.h>

namespace terminus
{

class Game;
class EventHandler;
class Train;
class AbstractWagon;
class QMLTrain;
class QMLWagon;

class UserInterface : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int currentWagonIndex READ currentWagonIndex NOTIFY currentWagonIndexChanged)

public:
    UserInterface(Game *game = nullptr);
    ~UserInterface();

    Q_INVOKABLE void keyPressEvent(Qt::Key key);
    Q_INVOKABLE void keyReleaseEvent(Qt::Key key);
    Q_INVOKABLE void mouseMoveEvent(qreal x, qreal y);
    Q_INVOKABLE void touchMoveEvent(qreal x, qreal y);
    Q_INVOKABLE void gyroMoveEvent(qreal x, qreal y);
    Q_INVOKABLE void switchToNextWagon();
    Q_INVOKABLE void switchToPreviousWagon();
    Q_INVOKABLE void touchChargeFire();
    Q_INVOKABLE void touchFire();

    /*Q_INVOKABLE void flickEvent(qreal startX, qreal x);
    Q_INVOKABLE void flickReset();*/

    Q_INVOKABLE QMLTrain *playerQMLTrain();
    Q_INVOKABLE QMLTrain *enemyQMLTrain();

    Q_INVOKABLE int currentWagonIndex();
    Q_INVOKABLE void sync();

signals:
    void gameChanged();
    void currentWagonIndexChanged();

protected:
    Game *m_game;
    std::unique_ptr<EventHandler> m_eventHandler;
    unsigned int m_lockedWagonIndex;
    int m_currentWagonIndex;
    QMLTrain *m_playerQMLTrain;
    QMLTrain *m_enemyQMLTrain;
};

}
