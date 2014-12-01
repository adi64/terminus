#pragma once

#include "abstractwagon.h"

#include <vector>

#include <glm/glm.hpp>

class QOpenGLBuffer;
class QOpenGLShaderProgram;
class QOpenGLVertexArrayObject;

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
    QOpenGLVertexArrayObject *m_vao;

    static const std::vector<glm::vec3> vertices();
    static const std::vector<unsigned short> indices();
};

}
