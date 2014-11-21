#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include <QObject>
#include "camera.h"

class Camera;
class QOpenGLShaderProgram;

namespace terminus
{

class AbstractGraphicsObject;

class Scene : public QObject
{
    Q_OBJECT
public:
    Scene();

    Camera & camera();
    void addNode(AbstractGraphicsObject* node);
public slots:
    void render();
protected:
private:
    Camera* m_camera;
    std::vector<AbstractGraphicsObject*> m_nodes;

};



}


#endif // SCENE_H
