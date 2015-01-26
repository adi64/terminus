#pragma once

#include <memory>

#include <QObject>
#include <QQuickItem>

#include "scene.h"
#include "game.h"

namespace terminus
{

class EventHandler;

class UserInterface : public QQuickItem
{
    Q_OBJECT

public:
    UserInterface();
    ~UserInterface();

    // Reference to game is sent with every signal because it cant be sent when UI is initialized
    Q_INVOKABLE void setGame(Game *game);
    Q_INVOKABLE void keyPressEvent(Qt::Key key);
    Q_INVOKABLE void keyReleaseEvent(Qt::Key key);
    Q_INVOKABLE void mouseMoveEvent(qreal x, qreal y);
    Q_INVOKABLE void touchMoveEvent(qreal x, qreal y);
    Q_INVOKABLE void gyroMoveEvent(qreal x, qreal y);
    /*Q_INVOKABLE void flickEvent(qreal startX, qreal x);
    Q_INVOKABLE void flickReset();*/
    Q_INVOKABLE void switchWagonLeft();
    Q_INVOKABLE void switchWagonRight();
    Q_INVOKABLE void touchChargeFire();
    Q_INVOKABLE void touchFire();

protected slots:
    void sync();
    void render();
    void cleanup();
    void handleWindowChanged(QQuickWindow* win);

protected:
    std::unique_ptr<EventHandler> m_eventHandler;
};

}
