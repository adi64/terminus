#pragma once

#include <Qt>

namespace terminus
{

class Application;

/*!
 * \brief The EventHandler class receives events (touch, key, mouse, gyroscope,
 * etc.) and triggers actions, taking object states and context into account.
 */
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
    EventHandler(Application *game = nullptr);

    /*!
     * \brief Handles a click on a QML UI button
     */
    void buttonInput(int type);

    /*!
     * \brief Handles a key event
     */
    void keyInput(Qt::Key key);

    /*!
     * \brief Handles mouse and gyroscope movements
     */
    void moveInput(int type, qreal x, qreal y);

    /*!
     * \brief Handles touch input
     */
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
    Application *m_game;
};

}
