#pragma once

#include "abstractgraphicsobject.h"

class QImage;
class QOpenGLTexture;

namespace terminus
{

class SkyBox : public AbstractGraphicsObject
{
public:
    SkyBox(Scene *scene);
    ~SkyBox();
protected:
    QImage *m_imageNX;
    QImage *m_imagePX;
    QImage *m_imageNY;
    QImage *m_imagePY;
    QImage *m_imageNZ;
    QImage *m_imagePZ;
    QOpenGLTexture *m_texture;
};

}
