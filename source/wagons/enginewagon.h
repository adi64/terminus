#pragma once

#include "abstractwagon.h"

class QOpenGLShaderProgram;

namespace terminus
{

class EngineWagon : public AbstractWagon
{
public:
    EngineWagon(Scene *scene, Train *train);
    void render(QOpenGLFunctions& gl, int elapsedMilliseconds);
    float length() const;

protected:
    QOpenGLShaderProgram *m_program;
};

}
