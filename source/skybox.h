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
    SkyBox(const std::shared_ptr<Scene> &scene);

    void render(QOpenGLFunctions & gl) const override;
protected:
    void initialize(QOpenGLFunctions & gl) const;

    mutable bool m_initialized;
    std::unique_ptr<QImage> m_imageNX;
    std::unique_ptr<QImage> m_imagePX;
    std::unique_ptr<QImage> m_imageNY;
    std::unique_ptr<QImage> m_imagePY;
    std::unique_ptr<QImage> m_imageNZ;
    std::unique_ptr<QImage> m_imagePZ;
    mutable GLuint m_texture;
};

}
