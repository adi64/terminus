#pragma once

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
    SkyBox(Scene *scene);
    ~SkyBox();

    void update(int elapsedMilliseconds);
    void render(QOpenGLFunctions & gl);
protected:
    void initialize(QOpenGLFunctions & gl);

    bool m_initialized;

    QImage *m_imageNX;
    QImage *m_imagePX;
    QImage *m_imageNY;
    QImage *m_imagePY;
    QImage *m_imageNZ;
    QImage *m_imagePZ;
    GLuint m_texture;
};

}
