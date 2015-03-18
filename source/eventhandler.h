#pragma once

#include <Qt>

namespace terminus
{

class Game;

class EventHandler
{

    enum InteractionType
    {
        MOUSE_MOVEMENT = 1,
        GYRO_MOVEMENT = 2,
        NEXT_WAGON_BUTTON = 3,
        PREV_WAGON_BUTTON = 4,
        ACTION_BUTTON = 5,
        LEFT_MOUSE_BUTTON = 6
    };

public:
    EventHandler(Game *game = nullptr);

    void buttonInput(int type);
    void keyInput(Qt::Key key);
    void moveInput(int type, qreal x, qreal y);
    void touchInput(qreal oldx, qreal oldy, qreal x, qreal y);

protected:
    void keyPressEvent(Qt::Key key);
    void mouseMoveEvent(qreal x, qreal y);
    void gyroMoveEvent(qreal x, qreal y);
    void switchToNextWagon();
    void switchToPreviousWagon();
    void touchChargeFire();
    void touchFire();

protected:
    Game *m_game;
};

}
