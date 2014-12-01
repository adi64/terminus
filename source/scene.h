#pragma once

#include <vector>

#include <QObject>
#include "camera.h"

class Camera;
class QOpenGLShaderProgram;

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
public slots:
    void render();
protected:
private:
    Camera* m_camera;
    Painter* m_painter;
    std::vector<AbstractGraphicsObject*> m_nodes;

};



}
