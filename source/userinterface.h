#pragma once

#include <memory>

#include <world/scene.h>

namespace terminus
{

class Game;
class EventHandler;
class Train;
class AbstractWagon;

class UserInterface
{

public:
    UserInterface();
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

protected:
    std::unique_ptr<EventHandler> m_eventHandler;
};

}
