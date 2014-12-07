#pragma once

#include <vector>

#include <QObject>
#include <QOpenGLFunctions>

#include "camera.h"

class QOpenGLShaderProgram;
class QTime;

namespace terminus
{

class AbstractGraphicsObject;
class Painter;

class Scene : public QObject
{
    Q_OBJECT
public:
    Scene();
    ~Scene();

    Camera & camera();

    void addNode(AbstractGraphicsObject* node);
    void setInitialTimeStamp(QTime *timeStamp);
public slots:
    void render();
protected:
private:
    Camera* m_camera;
    QOpenGLFunctions m_gl;
    std::vector<AbstractGraphicsObject*> m_nodes;
    QTime *m_timeStamp;

};



}
