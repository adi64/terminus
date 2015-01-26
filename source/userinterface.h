#pragma once

#include <memory>

#include <QObject>
#include <QQuickItem>

#include "scene.h"

namespace terminus
{

class Game;
class EventHandler;
class Train;
class AbstractWagon;

class UserInterface : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(float status READ status WRITE setStatus NOTIFY statusChanged)

public:
    UserInterface();
    ~UserInterface();

    Q_INVOKABLE void keyPressEvent(Qt::Key key);
    Q_INVOKABLE void keyReleaseEvent(Qt::Key key);
    Q_INVOKABLE void mouseMoveEvent(qreal x, qreal y);
    Q_INVOKABLE void touchMoveEvent(qreal x, qreal y);
    Q_INVOKABLE void gyroMoveEvent(qreal x, qreal y);
    Q_INVOKABLE void switchWagonLeft();
    Q_INVOKABLE void switchWagonRight();
    Q_INVOKABLE void touchChargeFire();
    Q_INVOKABLE void touchFire();

    /*Q_INVOKABLE void flickEvent(qreal startX, qreal x);
    Q_INVOKABLE void flickReset();*/

    Q_INVOKABLE void setStatus(float status);
    Q_INVOKABLE float status() const;
    Q_INVOKABLE void sync(Game *game);

signals:
    void statusChanged();

protected:
    std::unique_ptr<EventHandler> m_eventHandler;
    unsigned int m_lockedWagonIndex;
    AbstractWagon *m_currentWagon;
    float m_wagonStatus;
};

}
