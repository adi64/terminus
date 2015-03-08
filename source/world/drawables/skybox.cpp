#include "skybox.h"

#include <QImage>
#include <QOpenGLTexture>
#include <QDebug>

#include <world/world.h>
#include <resources/resourcemanager.h>
#include <resources/geometry.h>
#include <resources/program.h>

namespace terminus
{

SkyBox::SkyBox(World & world)
: AbstractGraphicsObject(world)
, m_cubeMapOnGPU(false)
{
    m_program = ResourceManager::getInstance()->getProgram("envmap");
    m_geometry = ResourceManager::getInstance()->getGeometry("base_ScreenQuad");

    m_imageNX = std::unique_ptr<QImage>(new QImage(":/data/env_cube_nx.png"));
    m_imagePX = std::unique_ptr<QImage>(new QImage(":/data/env_cube_px.png"));
    m_imageNY = std::unique_ptr<QImage>(new QImage(":/data/env_cube_ny.png"));
    m_imagePY = std::unique_ptr<QImage>(new QImage(":/data/env_cube_py.png"));
    m_imageNZ = std::unique_ptr<QImage>(new QImage(":/data/env_cube_nz.png"));
    m_imagePZ = std::unique_ptr<QImage>(new QImage(":/data/env_cube_pz.png"));
}

void SkyBox::localRenderSetup(QOpenGLFunctions & gl, Program & /*program*/) const
{
    allocateCubeMap(gl);

    gl.glActiveTexture(GL_TEXTURE0);
    gl.glEnable(GL_TEXTURE_CUBE_MAP);
    gl.glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);
}
void SkyBox::localRenderCleanup(QOpenGLFunctions & gl, Program & /*program*/) const
{
    gl.glActiveTexture(GL_TEXTURE0);
    gl.glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
void SkyBox::allocateCubeMap(QOpenGLFunctions &gl) const
{
    if(m_cubeMapOnGPU)
        return;

    m_cubeMap = -1;
    gl.glGenTextures(1, &m_cubeMap);
    gl.glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);

    gl.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gl.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    gl.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    gl.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //gl.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    gl.glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, m_imagePX->width(), m_imagePX->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_imagePX->bits());
    gl.glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, m_imageNX->width(), m_imageNX->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_imageNX->bits());
    gl.glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, m_imagePY->width(), m_imagePY->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_imagePY->bits());
    gl.glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, m_imageNY->width(), m_imageNY->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_imageNY->bits());
    gl.glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, m_imagePZ->width(), m_imagePZ->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_imagePZ->bits());
    gl.glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, m_imageNZ->width(), m_imageNZ->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_imageNZ->bits());

    gl.glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    m_cubeMapOnGPU = true;
}

void SkyBox::deallocateCubeMap(QOpenGLFunctions & gl) const
{
    if(!m_cubeMapOnGPU)
        return;

    gl.glDeleteTextures(1, &m_cubeMap);

    m_cubeMapOnGPU = false;
}

}
