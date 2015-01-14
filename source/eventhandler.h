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
    void flickEvent(qreal velo);

protected:
    Game *m_game;
    unsigned int m_lockedWagonIndex; //TODO Move to train
};

}
