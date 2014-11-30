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
    void render();
    static float length();

protected:
    void initCube();
    Scene *m_scene;
    QOpenGLShaderProgram *m_program;
    QOpenGLBuffer *m_vbo;
    QOpenGLBuffer *m_ibo;
};

}
