#pragma once

#include <Qt>

namespace terminus
{

class Game;

class EventHandler
{
public:
    EventHandler();

    void keyPressEvent(Qt::Key key, Game *game);
    void keyReleaseEvent(Qt::Key key, Game *game);
    void mouseMoveEvent(qreal x, qreal y, Game *game);
    void touchMoveEvent(qreal x, qreal y, Game *game);
    void gyroMoveEvent(qreal x, qreal y, Game *game);
    void flickEvent(qreal startx, qreal x, Game *game);
    void flickReset(Game *game);
    void touchChargeFire(Game *game);
    void touchFire(Game *game);

protected:
    unsigned int m_lockedWagonIndex; //TODO Move to train
    bool m_flicked;
    bool m_flickResetted;
    qreal m_flickDirection;
};

}
