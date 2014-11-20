#ifndef SQUIRCLE_H
#define SQUIRCLE_H

#include "abstractgraphicsobject.h"

class QOpenGLShaderProgram;

namespace Terminus
{

class Squircle : public AbstractGraphicsObject
{
public:
    Squircle();
    void render();
private:
    QOpenGLShaderProgram *m_program;
};

}

#endif // SQUIRCLE_H
