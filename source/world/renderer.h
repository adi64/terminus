#pragma once

#include <map>
#include <vector>

#include <resources/program.h>
#include <world/drawables/abstractgraphicsobject.h>

class QOpenGLFunctions;

namespace terminus
{

class Renderer
{
public:
    Renderer(World & world);
    void clear();
    void addGraphicsObject(AbstractGraphicsObject *object);
    void renderAll(QOpenGLFunctions & gl);
protected:
    World & m_world;
    std::map< Program*, std::vector< AbstractGraphicsObject* > > m_renderMap;
};

}
