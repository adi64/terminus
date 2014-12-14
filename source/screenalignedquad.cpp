#include <cassert>

#include "screenalignedquad.h"

namespace terminus
{

ScreenAlignedQuad::ScreenAlignedQuad(Scene *scene)
    : AbstractGraphicsObject(scene)
    , m_vertices(QOpenGLBuffer::VertexBuffer)
    , m_initialized(false)

{

}

void ScreenAlignedQuad::strip(QOpenGLBuffer & vertices)
{
	float rawv[] = { +1.f, -1.f , +1.f, +1.f , -1.f, -1.f , -1.f, +1.f };

	vertices.bind();
	vertices.allocate(rawv, sizeof(float)* 8);
}

void ScreenAlignedQuad::initialize(QOpenGLFunctions &gl)
{
    m_vao.create();
    m_vao.bind();

    m_vertices.create();
    m_vertices.setUsagePattern(QOpenGLBuffer::StaticDraw);

    strip(m_vertices);

    gl.glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 0, nullptr);
    gl.glEnableVertexAttribArray(0);

    m_vao.release();

    m_initialized = true;
}


void ScreenAlignedQuad::render(QOpenGLFunctions &gl, int elapsedMilliseconds)
{
    if(!m_initialized)
    {
        initialize(gl);
    }

    m_vao.bind();
    gl.glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    m_vao.release();
}

}
