#pragma once

#include <memory>

#include <QObject>
#include <QQuickItem>

class QTimer;
class QTime;

namespace terminus
{

class Train;
class Scene;
class Terrain;
class ResourceManager;

class Game : public QQuickItem
{
    Q_OBJECT

public:
    Game();
    ~Game();
public slots:
    void sync();
    void cleanup();
    void handleWindowChanged(QQuickWindow* win);
    void keyPressEvent(Qt::Key key);
    void keyReleaseEvent(Qt::Key key);
protected:
    Scene *m_scene;
    std::unique_ptr<Train> m_playerTrain;
    std::unique_ptr<Train> m_enemyTrain;
    QTimer *m_timer;
    QTime *m_timeStamp;
    std::unique_ptr<Terrain> m_terrain;
};

}
