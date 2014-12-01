#pragma once

#include <memory>

#include <QObject>
#include <QQuickItem>

class QTimer;

namespace terminus
{

class Train;
class Scene;

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
protected:
private:
    Scene *m_scene;
    std::unique_ptr<Train> m_playerTrain;
    QTimer *m_timer;
};

}
