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

GameObject::GameObject(Game & world)
: m_world(world)
, m_validState(true)
, m_camera(nullptr)
, m_matrix()
{
}

GameObject::~GameObject()
{
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
    m_world.localPlayer().camera().setUniforms(program, m_matrix.matrix());
    m_world.lightManager().setUniforms(program);
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

void GameObject::setPosition(const QVector3D & position)
{
    m_position = position;
    m_modelMatrixChanged = true;
    m_modelMatrixInvertedChanged = true;
}

void GameObject::setRotation(const QQuaternion &rotation)
{
    m_rotation = rotation;
    m_modelMatrixChanged = true;
    m_modelMatrixInvertedChanged = true;
}

void GameObject::setScale(const QVector3D & scale)
{
    m_scale = scale;
    m_modelMatrixChanged = true;
    m_modelMatrixInvertedChanged = true;
}

void GameObject::setScale(float scale)
{
    m_scale = QVector3D(scale, scale, scale);
    m_modelMatrixChanged = true;
    m_modelMatrixInvertedChanged = true;
}

QVector3D GameObject::worldToModel(const QVector3D & vWorld)
{
    QVector4D v4 = modelMatrixInverted() * QVector4D(vWorld, 1.f);
    return v4.toVector3DAffine();
}

QVector3D GameObject::modelToWorld(const QVector3D & vModel)
{
    QVector4D v4 = modelMatrix() * QVector4D(vModel, 1.f);
    return v4.toVector3DAffine();
}

void GameObject::dispose()
{
    if(!m_validState)
    {
        return;
    }

    m_world.scheduleAction(
        [this]()
        {
            m_world.deleteObject(this);
            return false;
        });
    m_validState = false;
}

} //namespace terminus
