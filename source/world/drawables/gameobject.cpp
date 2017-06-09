#include "gameobject.h"

#include <util/gldebug.h>

#include <player/localplayer.h>
#include <resources/geometry.h>
#include <resources/material.h>
#include <resources/program.h>
#include <world/lightmanager.h>
#include <player/camera.h>
#include <world/game.h>

namespace terminus
{

GameObject::GameObject(Game & game)
: m_game(game)
, m_validState(true)
, m_camera(nullptr)
, m_matrix()
{
}


const std::shared_ptr<std::unique_ptr<Program>> & GameObject::renderProgram() const
{
    return m_program;
}

const std::shared_ptr<std::unique_ptr<Geometry>> & GameObject::renderGeometry() const
{
    return m_geometry;
}

const std::shared_ptr<std::unique_ptr<Material>> & GameObject::renderMaterial() const
{
    return m_material;
}

void GameObject::update()
{
    if(!m_validState)
    {
        return;
    }

    localUpdate();

    if(m_camera)
    {
        adjustCamera();
    }

    doForAllChildren(
        [](GameObject & child)
        {
            child.update();
        });
}

void GameObject::render()
{
    if(!m_validState)
    {
        return;
    }

    if(localRenderEnabled())
    {
        localRender();
    }

    doForAllChildren(
        [](GameObject & child) mutable
        {
            child.render();
        });
}

void GameObject::unbindCamera(Camera * camera)
{
    if(camera != m_camera)
    {
        return;
    }
    if(camera)
    {
        m_camera->unbound(this);
    }
    m_camera = nullptr;
    onUnbindCamera();
}

void GameObject::bindCamera(Camera * camera)
{
    if(m_camera == camera)
    {
        return;
    }
    unbindCamera(m_camera);
    m_camera = camera;
    onBindCamera();
}

void GameObject::onBindCamera()
{
}

void GameObject::onUnbindCamera()
{
}

void GameObject::adjustCamera()
{
}

void GameObject::moveEvent(QVector3D /*movement*/)
{
}

void GameObject::rotateEvent(QVector2D /*rotation*/)
{
}

ObjectMatrix & GameObject::matrix()
{
    return m_matrix;
}

const ObjectMatrix & GameObject::matrix() const
{
    return m_matrix;
}

const QVector3D & GameObject::minBB() const
{
    static const QVector3D vZero{0.f, 0.f, 0.f};
    if(!m_geometry || !*m_geometry)
    {
        return vZero;
    }
    return (**m_geometry).bBoxMin();
}

const QVector3D & GameObject::maxBB() const
{
    static const QVector3D vZero{0.f, 0.f, 0.f};
    if(!m_geometry || !*m_geometry)
    {
        return vZero;
    }
    return (**m_geometry).bBoxMax();
}

QVector3D GameObject::worldUp()
{
    return m_matrix.transform({0.f, 1.f, 0.f});
}

QVector3D GameObject::worldFront()
{
    return m_matrix.transform({1.f, 0.f, 0.f});
}

QVector3D GameObject::worldSide()
{
    return m_matrix.transform({0.f, 0.f, 1.f});
}


void GameObject::localUpdate()
{
}

void GameObject::localRender() const
{
    if(!m_geometry || !*m_geometry)
    {
        qDebug() << "Geometry to render does not exist!";
        return;
    }
    if(!m_program || !*m_program)
    {
        qDebug() << "Program to render with does not exist!";
        return;
    }

    Geometry & geometry = **m_geometry;
    Program & program = **m_program;

    printGlError(__FILE__, __LINE__);
    program.bind();

    printGlError(__FILE__, __LINE__);
    //TODO-LW move to renderer.cpp: m_game.localPlayer().camera().setUniforms(program, m_matrix.matrix());
    m_game.lightManager().setUniforms(program);
    printGlError(__FILE__, __LINE__);

    if(m_material && *m_material)
    {
        (**m_material).setUniforms(program);
        printGlError(__FILE__, __LINE__);
    }

    localRenderSetup(program);
    printGlError(__FILE__, __LINE__);

    printGlError(__FILE__, __LINE__);
    geometry.draw();
    printGlError(__FILE__, __LINE__);

    localRenderCleanup(program);
    printGlError(__FILE__, __LINE__);

    program.release();
    printGlError(__FILE__, __LINE__);
}

void GameObject::localRenderSetup(Program & /*program*/) const
{
}

void GameObject::localRenderCleanup(Program & /*program*/) const
{
}

bool GameObject::localRenderEnabled() const
{
    return true;
}

void GameObject::doForAllChildren(std::function<void (GameObject &)> /*callback*/)
{
}

void GameObject::dispose()
{
    if(!m_validState) //TODO-LW: why is m_validState necessary
    {
        return;
    }

    m_game.scheduleAction(
        [this]()
        {
            m_game.deleteObject(this);
            return false;
        });
    m_validState = false;
}

} //namespace terminus
