#include "abstractgraphicsobject.h"

#include <QDebug>
#include <QQuaternion>

#include <player/localplayer.h>
#include <resources/program.h>
#include <world/lightmanager.h>
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

void AbstractGraphicsObject::render(QOpenGLFunctions & gl)
{
    if(!m_validState)
    {
        return;
    }

    if(localRenderEnabled())
    {
        localRender(gl);
    }

    doForAllChildren(
        [gl](AbstractGraphicsObject & child) mutable
        {
            child.render(gl);
        });
}

void AbstractGraphicsObject::unbindCamera(Camera * cam)
{
    if(cam != m_camera)
    {
        return;
    }
    if(cam)
    {
        m_camera->unbound(this);
    }
    m_camera = nullptr;
    onUnbindCamera();
}

void AbstractGraphicsObject::bindCamera(Camera * cam)
{
    if(m_camera == cam)
    {
        return;
    }
    unbindCamera(m_camera);
    m_camera = cam;
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

QVector3D AbstractGraphicsObject::worldLeft()
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
        m_modelMatrixInverted = m_modelMatrix.inverted();
        m_modelMatrixChanged = false;
    }
    return m_modelMatrix;
}

QMatrix4x4 AbstractGraphicsObject::modelMatrixInverted() const
{
    if(m_modelMatrixChanged)
    {
        m_modelMatrix.setToIdentity();
        m_modelMatrix.translate(position());
        m_modelMatrix.rotate(rotation());
        m_modelMatrix.scale(scale());
        m_modelMatrixInverted = m_modelMatrix.inverted();
        m_modelMatrixChanged = false;
    }
    return m_modelMatrixInverted;
}

void AbstractGraphicsObject::localUpdate()
{
}

void AbstractGraphicsObject::localRender(QOpenGLFunctions & gl) const
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

    program.bind();

    m_world.localPlayer().camera().setMatrices(program, modelMatrix());
    m_world.lightManager().setUniforms(program);

    if(m_material && *m_material)
    {
        (**m_material).setUniforms(program);
    }

    localRenderSetup(gl, program);

    geometry.setAttributes(program);
    geometry.draw(gl);

    localRenderCleanup(gl, program);

    program.release();
}

void AbstractGraphicsObject::localRenderSetup(QOpenGLFunctions & /*gl*/, Program & /*program*/) const
{
}

void AbstractGraphicsObject::localRenderCleanup(QOpenGLFunctions & /*gl*/, Program & /*program*/) const
{
}

bool AbstractGraphicsObject::localRenderEnabled() const
{
    return true;
}

void AbstractGraphicsObject::setPosition(const QVector3D & position)
{
    m_position = position;
    m_modelMatrixChanged = true;
}

void AbstractGraphicsObject::setRotation(const QQuaternion &eulerAngles)
{
    m_rotation = eulerAngles;
    m_modelMatrixChanged = true;
}

void AbstractGraphicsObject::setScale(const QVector3D & scale)
{
    m_scale = scale;
    m_modelMatrixChanged = true;
}

void AbstractGraphicsObject::setScale(float scale)
{
    m_scale = QVector3D(scale, scale, scale);
    m_modelMatrixChanged = true;
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

void AbstractGraphicsObject::doForAllChildren(std::function<void (AbstractGraphicsObject &)> /*callback*/)
{
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

}
