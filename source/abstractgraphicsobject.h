#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLFunctions>

namespace terminus
{

class Scene;

class AbstractGraphicsObject
{
public:
    AbstractGraphicsObject(Scene* scene);
    virtual void render() = 0;
protected:
    Scene *m_scene;
};

}
