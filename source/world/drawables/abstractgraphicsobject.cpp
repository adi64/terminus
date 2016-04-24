#include "abstractgraphicsobject.h"

#include <util/gldebug.h>

#include <player/localplayer.h>
#include <resources/geometry.h>
#include <resources/material.h>
#include <resources/program.h>
#include <world/lightmanager.h>
#include <world/camera.h>
#include <world/world.h>

namespace terminus
{

AbstractGraphicsObject::AbstractGraphicsObject(World & world)
: m_world(world)
, m_validState(true)
, m_camera(nullptr)
, m_position(0.0, 0.0, 0.0)
, m_rotation(1.0, 0.0, 0.0, 0.0)
, m_scale(1.0, 1.0, 1.0)
, m_modelMatrixChanged(false)
, m_modelMatrixInvertedChanged(false)
{
}

AbstractGraphicsObject::~AbstractGraphicsObject()
{
}

void AbstractGraphicsObject::update()
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
        [](AbstractGraphicsObject & child)
        {
            child.update();
        });
}

void AbstractGraphicsObject::render()
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
        [](AbstractGraphicsObject & child) mutable
        {
            child.render();
        });
}

void AbstractGraphicsObject::unbindCamera(Camera * camera)
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

void AbstractGraphicsObject::bindCamera(Camera * camera)
{
    if(m_camera == camera)
    {
        return;
    }
    unbindCamera(m_camera);
    m_camera = camera;
    onBindCamera();
}

void AbstractGraphicsObject::onBindCamera()
{
}

void AbstractGraphicsObject::onUnbindCamera()
{
}

void AbstractGraphicsObject::adjustCamera()
{
}

void AbstractGraphicsObject::moveEvent(QVector3D /*movement*/)
{
}

void AbstractGraphicsObject::rotateEvent(QVector2D /*rotation*/)
{
}

const QVector3D & AbstractGraphicsObject::minBB() const
{
    static const QVector3D vZero{0.f, 0.f, 0.f};
    if(!m_geometry || !*m_geometry)
    {
        return vZero;
    }
    return (**m_geometry).bBoxMin();
}

const QVector3D & AbstractGraphicsObject::maxBB() const
{
    static const QVector3D vZero{0.f, 0.f, 0.f};
    if(!m_geometry || !*m_geometry)
    {
        return vZero;
    }
    return (**m_geometry).bBoxMax();
}

QVector3D AbstractGraphicsObject::worldUp()
{
    return (modelToWorld({0.f, 1.f, 0.f}) - position()).normalized();
}

QVector3D AbstractGraphicsObject::worldFront()
{
    return (modelToWorld({1.f, 0.f, 0.f}) - position()).normalized();
}

QVector3D AbstractGraphicsObject::worldSide()
{
    return (modelToWorld({0.f, 0.f, 1.f}) - position()).normalized();
}

QVector3D AbstractGraphicsObject::position() const
{
    return m_position;
}
QQuaternion AbstractGraphicsObject::rotation() const
{
    return m_rotation;
}
QVector3D AbstractGraphicsObject::scale() const
{
    return m_scale;
}

QMatrix4x4 AbstractGraphicsObject::modelMatrix() const
{
    if(m_modelMatrixChanged)
    {
        m_modelMatrix.setToIdentity();
        m_modelMatrix.translate(position());
        m_modelMatrix.rotate(rotation());
        m_modelMatrix.scale(scale());
        m_modelMatrixChanged = false;
    }
    return m_modelMatrix;
}

QMatrix4x4 AbstractGraphicsObject::modelMatrixInverted() const
{
    if(m_modelMatrixInvertedChanged)
    {
        m_modelMatrixInverted = modelMatrix().inverted();
        m_modelMatrixInvertedChanged = false;
    }
    return m_modelMatrixInverted;
}

void AbstractGraphicsObject::localUpdate()
{
}

void AbstractGraphicsObject::localRender() const
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
    m_world.localPlayer().camera().setUniforms(program, modelMatrix());
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

void AbstractGraphicsObject::localRenderSetup(Program & /*program*/) const
{
}

void AbstractGraphicsObject::localRenderCleanup(Program & /*program*/) const
{
}

bool AbstractGraphicsObject::localRenderEnabled() const
{
    return true;
}

void AbstractGraphicsObject::doForAllChildren(std::function<void (AbstractGraphicsObject &)> /*callback*/)
{
}

void AbstractGraphicsObject::setPosition(const QVector3D & position)
{
    m_position = position;
    m_modelMatrixChanged = true;
    m_modelMatrixInvertedChanged = true;
}

void AbstractGraphicsObject::setRotation(const QQuaternion &rotation)
{
    m_rotation = rotation;
    m_modelMatrixChanged = true;
    m_modelMatrixInvertedChanged = true;
}

void AbstractGraphicsObject::setScale(const QVector3D & scale)
{
    m_scale = scale;
    m_modelMatrixChanged = true;
    m_modelMatrixInvertedChanged = true;
}

void AbstractGraphicsObject::setScale(float scale)
{
    m_scale = QVector3D(scale, scale, scale);
    m_modelMatrixChanged = true;
    m_modelMatrixInvertedChanged = true;
}

QVector3D AbstractGraphicsObject::worldToModel(const QVector3D & vWorld)
{
    QVector4D v4 = modelMatrixInverted() * QVector4D(vWorld, 1.f);
    return v4.toVector3DAffine();
}

QVector3D AbstractGraphicsObject::modelToWorld(const QVector3D & vModel)
{
    QVector4D v4 = modelMatrix() * QVector4D(vModel, 1.f);
    return v4.toVector3DAffine();
}

void AbstractGraphicsObject::dispose()
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
