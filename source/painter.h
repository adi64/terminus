#pragma once

#include <QOpenGLFunctions>

namespace terminus
{

class Painter : public QOpenGLFunctions
{
public:
    Painter();
    void initialize();
};

}
