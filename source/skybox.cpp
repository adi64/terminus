#include "skybox.h"

#include <QImage>
#include <QOpenGLTexture>

namespace terminus
{

SkyBox::SkyBox(Scene *scene)
    : AbstractGraphicsObject(scene)
{
    m_imageNX = new QImage("data/env_cube_nx.png");
    m_imagePX = new QImage("data/env_cube_px.png");
    m_imageNY = new QImage("data/env_cube_ny.png");
    m_imagePY = new QImage("data/env_cube_py.png");
    m_imageNZ = new QImage("data/env_cube_nz.png");
    m_imagePZ = new QImage("data/env_cube_pz.png");

    m_texture = new QOpenGLTexture;
    m_texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeX, m_imageNX->format(), QOpenGLTexture::Int8, m_imageNX->constBits());
    m_texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveX, m_imagePX->format(), QOpenGLTexture::Int8, m_imagePX->constBits());
    m_texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeY, m_imageNY->format(), QOpenGLTexture::Int8, m_imageNY->constBits());
    m_texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveY, m_imagePY->format(), QOpenGLTexture::Int8, m_imagePY->constBits());
    m_texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeZ, m_imageNZ->format(), QOpenGLTexture::Int8, m_imageNZ->constBits());
    m_texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveZ, m_imagePZ->format(), QOpenGLTexture::Int8, m_imagePZ->constBits());
}

SkyBox::~SkyBox()
{

}


}
