#include "skybox.h"

#include <QImage>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QDebug>

#include "scene.h"
#include "screenalignedquad.h"

namespace terminus
{

SkyBox::SkyBox(Scene *scene)
    : AbstractGraphicsObject(scene)
    , m_screenAlignedQuad(new ScreenAlignedQuad(scene))
    , m_initialized(false)
    , m_program(nullptr)
{

}

SkyBox::~SkyBox()
{

}

void SkyBox::render(QOpenGLFunctions &gl, int elapsedMilliseconds)
{
    if(!m_initialized)
    {
        initialize(gl);
    }

    if (!m_program)
    {
        m_program = new QOpenGLShaderProgram();

        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "data/envmap.vert");
        m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "data/envmap.frag");

        m_program->link();
    }

    m_program->bind();

    m_program->setUniformValue("cameraProjectionInverted", m_scene->camera().projectionInverted());
    m_program->setUniformValue("cameraView", m_scene->camera().view());
    m_program->setUniformValue("cubemap", 0);

    gl.glActiveTexture(GL_TEXTURE0);
    gl.glEnable(GL_TEXTURE_CUBE_MAP);
    gl.glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

    m_screenAlignedQuad->render(gl, elapsedMilliseconds);

    gl.glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    m_program->release();
}

void SkyBox::initialize(QOpenGLFunctions &gl)
{
    m_imageNX = new QImage("data/env_cube_nx.png");
    m_imagePX = new QImage("data/env_cube_px.png");
    m_imageNY = new QImage("data/env_cube_ny.png");
    m_imagePY = new QImage("data/env_cube_py.png");
    m_imageNZ = new QImage("data/env_cube_nz.png");
    m_imagePZ = new QImage("data/env_cube_pz.png");

    m_texture = -1;
    gl.glGenTextures(1, &m_texture);
    gl.glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

    gl.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gl.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    gl.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    gl.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    gl.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    gl.glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, m_imagePX->width(), m_imagePX->height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, m_imagePX->bits());
    gl.glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, m_imageNX->width(), m_imageNX->height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, m_imageNX->bits());
    gl.glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, m_imagePY->width(), m_imagePY->height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, m_imagePY->bits());
    gl.glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, m_imageNY->width(), m_imageNY->height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, m_imageNY->bits());
    gl.glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, m_imagePZ->width(), m_imagePZ->height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, m_imagePZ->bits());
    gl.glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, m_imageNZ->width(), m_imageNZ->height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, m_imageNZ->bits());

    gl.glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    qDebug() << "texture: " << m_texture;

    m_initialized = true;
}


}
