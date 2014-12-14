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
    m_program->setUniformValue("cameraProjectionInverted", m_scene->camera().view());

    m_screenAlignedQuad->render(gl, elapsedMilliseconds);

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

    m_texture = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);
    m_texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeX, QOpenGLTexture::RGB, QOpenGLTexture::UInt32_RGBA8, m_imageNX->constBits());
    m_texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveX, QOpenGLTexture::RGB, QOpenGLTexture::UInt32_RGBA8, m_imagePX->constBits());
    m_texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeY, QOpenGLTexture::RGB, QOpenGLTexture::UInt32_RGBA8, m_imageNY->constBits());
    m_texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveY, QOpenGLTexture::RGB, QOpenGLTexture::UInt32_RGBA8, m_imagePY->constBits());
    m_texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeZ, QOpenGLTexture::RGB, QOpenGLTexture::UInt32_RGBA8, m_imageNZ->constBits());
    m_texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveZ, QOpenGLTexture::RGB, QOpenGLTexture::UInt32_RGBA8, m_imagePZ->constBits());

    m_initialized = true;
}


}
