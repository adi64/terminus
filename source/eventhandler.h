#pragma once

#include <Qt>

namespace terminus
{

class Game;

class EventHandler
{
public:
    EventHandler(Game *game = nullptr);

    void keyPressEvent(Qt::Key key);
    void keyReleaseEvent(Qt::Key key);
    void mouseMoveEvent(qreal x, qreal y);
    void touchMoveEvent(qreal x, qreal y);
    void gyroMoveEvent(qreal x, qreal y);
    void switchToNextWagon();
    void switchToPreviousWagon();
    void touchChargeFire();
    void touchFire();

protected:
    Game *m_game;
    bool m_flicked;
    bool m_flickResetted;
    qreal m_flickDirection;
};

}
