#pragma once

#include "abstractgraphicsobject.h"

class QOpenGLShaderProgram;

namespace terminus
{

class Squircle : public AbstractGraphicsObject
{
public:
    Squircle();
    void render();
private:
    QOpenGLShaderProgram *m_program;
};

}
