#pragma once

#include "abstractwagon.h"
#include <QOpenGLShaderProgram>

class QOpenGLBuffer;

namespace terminus
{

class EngineWagon : public AbstractWagon
{
public:
    EngineWagon(Scene *scene);
    void render(QOpenGLFunctions& gl);
    static float length();

protected:
    void initCube(QOpenGLFunctions& gl);
    QOpenGLShaderProgram *m_program;
    QOpenGLBuffer *m_vbo;
    QOpenGLBuffer *m_ibo;
};

}
