#pragma once

#include <Qt>

namespace terminus
{

class Game;

class EventHandler
{
public:
    EventHandler(Game *game);

    void keyPressEvent(Qt::Key key);
    void keyReleaseEvent(Qt::Key key);
    void mouseMoveEvent(qreal x, qreal y);
    void touchMoveEvent(qreal x, qreal y);
    void gyroMoveEvent(qreal x, qreal y);
    void flickEvent(qreal startx, qreal x);
    void flickReset();
    void touchChargeFire();
    void touchFire();

protected:
    Game *m_game;
    bool m_flicked;
    bool m_flickResetted;
    qreal m_flickDirection;
};

}
