#include "abstractgraphicsobject.h"

#include <QQuaternion>
#include <QDebug>

#include <resources/lightmanager.h>
#include <player/localplayer.h>
#include <world/world.h>

namespace terminus
{

AbstractGraphicsObject::AbstractGraphicsObject(World & world)
: m_world(world)
, m_position(0.0, 0.0, 0.0)
, m_rotation(1.0, 0.0, 0.0, 0.0)
, m_scale(1.0, 1.0, 1.0)
, m_lockedEyeAngle(QQuaternion())
, m_modelMatrixChanged(false)
{
}

AbstractGraphicsObject::~AbstractGraphicsObject()
{

}

void AbstractGraphicsObject::update()
{
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
    if(cam == m_camera)
    {
        if(cam)
        {
            m_camera->unbound(this);
        }
    m_camera = nullptr;
    }
}

void AbstractGraphicsObject::bindCamera(Camera * cam)
{
    if(m_camera == cam)
    {
        return;
    }
    unbindCamera(m_camera);
    m_camera = cam;
}

void AbstractGraphicsObject::adjustCamera()
{
}

void AbstractGraphicsObject::moveEvent(QVector3D /*movement*/)
{
}

void AbstractGraphicsObject::rotateEvent(QVector2D rotation)
{
    Camera & camera = *m_camera;
    auto viewDirection = (camera.center() - camera.eye()).normalized();
    auto viewNormal = QVector3D::normal(viewDirection, camera.up());
    // "x rotation" -> rotate around up vector
    auto rotation_x = QQuaternion::fromAxisAndAngle(camera.up(), -rotation.x());
    // "y rotation" -> rotation around "the vector pointing to the right"
    auto rotation_y = QQuaternion::fromAxisAndAngle(viewNormal, -rotation.y());
    auto rotation_total = rotation_x * rotation_y;
    m_lockedEyeAngle *= rotation_total;
}

QVector3D AbstractGraphicsObject::worldUp()
{
    return m_rotation.rotatedVector(QVector3D(0.f, 1.f, 0.f)).normalized();
}

QVector3D AbstractGraphicsObject::worldFront()
{
    return m_rotation.rotatedVector(QVector3D(1.f, 0.f, 0.f)).normalized();
}

QVector3D AbstractGraphicsObject::worldRight()
{
    return m_rotation.rotatedVector(QVector3D(0.f, 0.f, 1.f)).normalized();
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

void AbstractGraphicsObject::localRenderSetup(QOpenGLFunctions & gl, Program & program) const
{
}

void AbstractGraphicsObject::localRenderCleanup(QOpenGLFunctions & gl, Program & program) const
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

void AbstractGraphicsObject::doForAllChildren(std::function<void (AbstractGraphicsObject &)> callback)
{
}

}
