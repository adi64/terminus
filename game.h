#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QQuickItem>

namespace terminus
{

class Scene;
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
protected:
private:
    Scene* m_scene;
    ResourceManager* m_resourceManager;
};

}

#endif // GAME_H
