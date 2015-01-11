#pragma once

#include <vector>
#include <memory>

#include <QObject>
#include <QOpenGLFunctions>

#include "camera.h"

class QOpenGLShaderProgram;
class QTime;

class btDiscreteDynamicsWorld;

namespace terminus
{

class AbstractGraphicsObject;
class Painter;

class Scene : public QObject
{
    Q_OBJECT
public:
    Scene(std::shared_ptr<btDiscreteDynamicsWorld> bulletWorld);
    ~Scene();

    void update(int elapsedMilliseconds);
    void render();

    Camera & camera();
    btDiscreteDynamicsWorld* bullet_world();

    void addNode(AbstractGraphicsObject* node);
    void deleteNode(AbstractGraphicsObject* node);

    void setInitialTimeStamp(QTime *timeStamp);
protected:
private:
    Camera* m_camera;
    QOpenGLFunctions m_gl;
    std::vector<AbstractGraphicsObject*> m_nodes;
    QTime *m_timeStamp;
    std::shared_ptr<btDiscreteDynamicsWorld> m_bullet_world;

};



}
