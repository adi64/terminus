#pragma once

#include <QOpenGLFunctions>

namespace terminus
{

class Scene;

class AbstractGraphicsObject
{
public:
    AbstractGraphicsObject(Scene* scene);
    virtual void render(QOpenGLFunctions& gl) = 0;
protected:
    Scene *m_scene;
};

}
