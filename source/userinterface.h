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
    Q_INVOKABLE void keyPressEvent(Qt::Key key, Game *game);
    Q_INVOKABLE void keyReleaseEvent(Qt::Key key, Game *game);
    Q_INVOKABLE void mouseMoveEvent(qreal x, qreal y, Game *game);
    Q_INVOKABLE void touchMoveEvent(qreal x, qreal y, Game *game);
    Q_INVOKABLE void gyroMoveEvent(qreal x, qreal y, Game *game);
    Q_INVOKABLE void flickEvent(qreal startX, qreal x, Game *game);
    Q_INVOKABLE void flickReset(Game *game);
    Q_INVOKABLE void touchChargeFire(Game *game);
    Q_INVOKABLE void touchFire(Game *game);

protected slots:
    void sync();
    void render();
    void cleanup();
    void handleWindowChanged(QQuickWindow* win);

protected:
    std::unique_ptr<EventHandler> m_eventHandler;
};

}
