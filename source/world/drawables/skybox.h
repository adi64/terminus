#pragma once

#include <memory>
#include "abstractgraphicsobject.h"

class QImage;
class QOpenGLTexture;
class QOpenGLFunctions;

namespace terminus
{

class ScreenAlignedQuad;

class SkyBox : public AbstractGraphicsObject
{
public:
    SkyBox(std::shared_ptr<Scene> scene);

    void preRender(QOpenGLFunctions & gl, Program & program) const override;
    void postRender(QOpenGLFunctions & gl, Program & program) const override;
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
