#pragma once

#include <memory>

#include <QObject>
#include <QQuickItem>

#include <world/scene.h>

namespace terminus
{

class Game;
class EventHandler;
class Train;
class AbstractWagon;

class UserInterface : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(float charge READ charge NOTIFY chargeChanged)
    Q_PROPERTY(float reload READ reload NOTIFY reloadChanged)
    Q_PROPERTY(QString wagonType READ wagonType NOTIFY wagonTypeChanged)

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

    Q_INVOKABLE float charge() const;
    Q_INVOKABLE float reload() const;
    Q_INVOKABLE QString wagonType() const;
    Q_INVOKABLE void sync(Game *game);

signals:
    void chargeChanged();
    void reloadChanged();
    void wagonTypeChanged();

protected:
    std::unique_ptr<EventHandler> m_eventHandler;
    unsigned int m_lockedWagonIndex;
    AbstractWagon *m_currentWagon;
    float m_charge;
    float m_reload;
    QString m_wagonType;
};

}
