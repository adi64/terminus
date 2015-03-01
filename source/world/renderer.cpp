#include "renderer.h"

#include <exception>

#include <QOpenGLFunctions>
#include <QDebug>

#include <resources/program.h>

namespace terminus
{

Renderer::Renderer(World &world)
: m_world(world)
{

}

void Renderer::clear()
{
    m_renderMap.clear();
}

void Renderer::addGraphicsObject(AbstractGraphicsObject *object)
{
    auto program = object->m_program.get()->get();
    try
    {
        m_renderMap.at(program).push_back(object);
        qDebug() << "Inserting object " << object << " with program " << program;
    }
    catch (std::exception)
    {
        auto newVector = std::vector<AbstractGraphicsObject*>();
        newVector.push_back(object);
        m_renderMap.insert(program, newVector);
        qDebug() << "Creating new vector for program " << program << ", caused by object " << object;
    }
}

void Renderer::renderAll(QOpenGLFunctions &gl)
{
    for(auto pair : m_renderMap)
    {
        auto program = *(pair.first);
        qDebug() << "Rendering with program " << program;

        program.bind();

        world.localPlayer().camera().setMatrices(program, modelMatrix());

        for(auto object : pair.second)
        {
            Material & material = **(object->m_material);
            material.setUniforms(program);

            object->localRenderSetup(gl, program);

            Geometry & geometry = **(object->m_geometry);
            geometry.setAttributes(program);
            geometry.draw(gl);

            object->localRenderCleanup(gl, program);
        }

        program.release();
    }
}

}
