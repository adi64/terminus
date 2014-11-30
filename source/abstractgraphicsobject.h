#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLFunctions>

namespace terminus
{

class AbstractGraphicsObject
{
public:
    AbstractGraphicsObject();
    virtual void render() = 0;
};

}
