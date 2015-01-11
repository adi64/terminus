#pragma once

#include <memory>
#include <functional>
#include <mutex>

#include <QObject>
#include <QQuickItem>

class QTimer;
class QTime;

using ActionList = std::vector<std::function<void(void)>>;

namespace terminus
{

class Train;
class Scene;
class EventHandler;
class Terrain;
class ResourceManager;
class SkyBox;

class Game : public QQuickItem
{
    Q_OBJECT

public:
    Game();
    ~Game();

    Scene *scene() const;
    Train *playerTrain() const;
    void scheduleEvent(std::function<void(void)> event);
public slots:
    void sync();
    void render();
    void cleanup();
    void handleWindowChanged(QQuickWindow* win);
    void keyPressEvent(Qt::Key key);
    void keyReleaseEvent(Qt::Key key);
    void mouseMoveEvent(qreal x, qreal y);
    void touchMoveEvent(qreal x, qreal y);
    void gyroMoveEvent(qreal x, qreal y);
    void flickEvent(qreal velo);
protected:
    Scene *m_scene;
    std::unique_ptr<EventHandler> m_eventHandler;
    std::unique_ptr<Train> m_playerTrain;
    std::unique_ptr<Train> m_enemyTrain;
    QTimer *m_timer;
    QTime *m_timeStamp;
    std::unique_ptr<Terrain> m_terrain;
    std::unique_ptr<SkyBox> m_skybox;
    std::unique_ptr<ActionList> m_actionList;
    std::mutex m_actionListMutex;
};

}
