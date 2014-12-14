#pragma once

#include <string>
#include <vector>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

#include "indextriple.h"

namespace terminus
{

struct Vertex{
    GLfloat position[3];
    GLfloat texCoord[3];
    GLfloat normal[3];
};

class Geometry
{

public:
    Geometry(); //construct NULL-Object --- what do we need this for?
    Geometry(const std::vector<unsigned int> & indexBuffer, const std::vector<Vertex> & vertexBuffer);
    virtual ~Geometry();

    virtual void allocate();
    virtual void deallocate();

    virtual void setAttributes(QOpenGLShaderProgram & program);
    virtual void draw(QOpenGLFunctions& gl);

protected:
    bool m_isOnGPU;
    //QOpenGLVertexArrayObject * m_vao;
    QOpenGLBuffer * m_vbo;
    QOpenGLBuffer * m_ibo;

    unsigned int m_triangleCount;

    std::vector<Vertex> m_vertexBuffer;
    std::vector<unsigned int> m_indexBuffer;
};

} //terminus
