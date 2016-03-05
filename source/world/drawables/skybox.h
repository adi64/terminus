#pragma once

#include <memory>

#include <world/drawables/abstractgraphicsobject.h>

class QImage;
class QOpenGLFunctions;

namespace terminus
{

class ScreenAlignedQuad;

/*!
 * \brief The SkyBox class renders an environment map in the form of a cube map
 * on a screen aligned quad in the background.
 */
class SkyBox : public AbstractGraphicsObject
{
public:
    explicit SkyBox(World & world);

    void localRenderSetup(QOpenGLFunctions & gl, Program & program) const override;
    void localRenderCleanup(QOpenGLFunctions & gl, Program & program) const override;
protected:
    void allocateCubeMap(QOpenGLFunctions & gl) const;
    void deallocateCubeMap(QOpenGLFunctions & gl) const;

    mutable bool m_cubeMapOnGPU;
    std::unique_ptr<QImage> m_imageNX;
    std::unique_ptr<QImage> m_imagePX;
    std::unique_ptr<QImage> m_imageNY;
    std::unique_ptr<QImage> m_imagePY;
    std::unique_ptr<QImage> m_imageNZ;
    std::unique_ptr<QImage> m_imagePZ;
    mutable GLuint m_cubeMap;
};

}
