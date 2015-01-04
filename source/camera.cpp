#include "camera.h"
#include <cassert>
#include <QQuaternion>

#include "abstractgraphicsobject.h"

namespace terminus
{

Camera::Camera(
    const QVector3D & eye
,   const QVector3D & center
,   const QVector3D & up)
: m_lockedToTrain(false)
, m_eye(eye)
, m_center(center)
, m_up(up)

, m_fovy(70.f) // degrees!
, m_aspect(16.f / 9.f)
, m_zNear(0.2f)
, m_zFar(1024.0f)
, m_viewChanged(true)
, m_viewInvertedChanged(true)
, m_projectionChanged(true)
, m_projectionInvertedChanged(true)
, m_viewProjectionChanged(true)
, m_viewProjectionInvertedChanged(true)
, m_normalChanged(true)
, m_lockedCenterOffset(QVector3D(0.0, 1.0, 0.0))
, m_lockedEyeOffset(QVector3D(0.0, 2.0, -2.0))
{
}

Camera::~Camera()
{
}

void Camera::invalidateMatrices() const
{
    m_viewChanged = true;
    m_viewInvertedChanged = true;
    m_projectionChanged = true;
    m_projectionInvertedChanged = true;
    m_viewProjectionChanged = true;
    m_viewProjectionInvertedChanged = true;
    m_normalChanged = true;
}

const QVector3D & Camera::eye() const
{
    return m_eye;
}

void Camera::setEye(const QVector3D & eye)
{
    if (eye == m_eye)
    {
        return;
    }

    m_eye = eye;
    invalidateMatrices();
}

const QVector3D & Camera::center() const
{
    return m_center;
}

void Camera::setCenter(const QVector3D & center)
{
    if (center == m_center)
    {
        return;
    }

    m_center = center;
    invalidateMatrices();
}

const QVector3D & Camera::up() const
{
    return m_up;
}

void Camera::setUp(const QVector3D & up)
{
    if (up == m_up)
    {
        return;
    }

    m_up = up;
    invalidateMatrices();
}

float Camera::zNear() const
{
    return m_zNear;
}

void Camera::setZNear(const float zNear)
{
    if (std::abs(zNear - m_zNear) < std::numeric_limits<float>::epsilon())
    {
        return;
    }

    m_zNear = zNear;
    assert(m_zNear > 0.0);

    invalidateMatrices();
}

float Camera::zFar() const
{
    return m_zFar;
}

void Camera::setZFar(const float zFar)
{
    if (std::abs(zFar - m_zFar) < std::numeric_limits<float>::epsilon())
    {
        return;
    }

    m_zFar = zFar;
    assert(m_zFar > m_zNear);

    invalidateMatrices();
}

float Camera::fovy() const
{
    return m_fovy;
}

void Camera::setFovy(const float fovy)
{
    if (std::abs(fovy - m_fovy) < std::numeric_limits<float>::epsilon())
    {
        return;
    }

    m_fovy = fovy;
    assert(m_fovy > 0.0);

    invalidateMatrices();
}

const QVector2D & Camera::viewport() const
{
    return m_viewport;
}

void Camera::setViewport(int width, int height)
{
    return setViewport(QVector2D(width, height));
}

void Camera::setViewport(const QVector2D & viewport)
{
    if (viewport == m_viewport)
    {
        return;
    }

    m_aspect = static_cast<float>(viewport.x()) / std::max(static_cast<float>(viewport.y()), 1.f);
    m_viewport = viewport;

    invalidateMatrices();
}

float Camera::aspectRatio() const
{
    return m_aspect;
}

QVector3D Camera::movement()
{
    return m_movement;
}

QVector2D Camera::rotation()
{
    return m_rotation;
}

void Camera::setMovement(QVector3D movement)
{
    if(!m_lockedToTrain)
    {
        m_movement = movement;

        auto direction = (center() - eye()).normalized();
        auto newEye = eye();
        auto newCenter = center();
        auto normal = QVector3D::normal(direction, up());

        newEye += normal * movement.x();
        newCenter += normal * movement.x();

        newEye += up() * movement.y();
        newCenter += up() * movement.y();

        newEye += direction * -movement.z();
        newCenter += direction * -movement.z();

        setEye(newEye);
        setCenter(newCenter);
    }
    else
    {
          // no movement just jump from wagon to wagon (arrows, numbers) and zoom (wasd?)
          //for that camera has to get wagon->position()
    }
}

void Camera::setRotation(QVector2D rotation)
{
    if(!m_lockedToTrain)
    {
        m_rotation = rotation;

        auto viewDirection = (center() - eye()).normalized();
        auto viewNormal = QVector3D::normal(viewDirection, up());

        // "x rotation" -> rotate around up vector
        auto rotation_x = QQuaternion::fromAxisAndAngle(up(), -rotation.x());

        // "y rotation" -> rotation around "the vector pointing to the right"
        auto rotation_y = QQuaternion::fromAxisAndAngle(viewNormal, rotation.y());

        auto rotation_total = rotation_x * rotation_y;

        auto newCenter = eye() + rotation_total.rotatedVector(viewDirection);

        setCenter(newCenter);
    }
    else
    {
        // TODO Camera Center should stay the same and eye should change

        // move along the "object x axis"

        // where does the "x vector" of that object really point to?

        auto objectLocalX = QVector3D(1.0, 0.0, 0.0);
        auto objectLocalZ = QVector3D(0.0, 0.0, 1.0);

        // "x rotation" -> rotate around up vector
        auto rotation_x = QQuaternion::fromAxisAndAngle(up(), -1.0 * m_lockedObject->eulerAngles().x());

        // "y rotation" -> rotation around "the vector pointing to the right"
        auto rotation_y = QQuaternion::fromAxisAndAngle(objectLocalZ, m_lockedObject->eulerAngles().y());

        auto rotation_total = rotation_x * rotation_y;
        auto objectGlobalX = rotation_total.rotatedVector(objectLocalX);

        auto newLockedEyeOffset = m_lockedEyeOffset + (objectGlobalX * rotation.x() * 0.15) + (up() * rotation.y() * -0.15);

        // stop at object's bounds
        // TODO FIXME: this needs the object's AABB and at the moment does not use object-local coordinates
        if(newLockedEyeOffset.x() > -3.0 && newLockedEyeOffset.x() < 3.0 && newLockedEyeOffset.y() > 0.0 && newLockedEyeOffset.y() < 4.0)
        {
            m_lockedEyeOffset = newLockedEyeOffset;
        }

    }
}

void Camera::toggleLocked()
{
    m_lockedToTrain = !m_lockedToTrain;
}

void Camera::setLocked(bool value)
{
    m_lockedToTrain = value;
}

bool Camera::isLocked() const
{
    return m_lockedToTrain;
}

void Camera::lockToObject(AbstractGraphicsObject *object)
{
    m_lockedObject = object;
    setLocked(true);
}

const QMatrix4x4 & Camera::view() const
{
    if (m_viewChanged)
    {
        m_view.setToIdentity();
        m_view.lookAt(m_eye, m_center, m_up);
    }
    m_viewChanged = false;

    return m_view;
}

const QMatrix4x4 & Camera::projection() const
{
    if (m_projectionChanged)
    {
        m_projection.setToIdentity();
        m_projection.perspective(m_fovy, m_aspect, m_zNear, m_zFar);
    }
    m_projectionChanged = false;

    return m_projection;
}

const QMatrix4x4 & Camera::viewProjection() const
{
    if (m_viewProjectionChanged)
    {
        m_viewProjection = projection() * view();
    }
    m_viewProjectionChanged = false;

    return m_viewProjection;
}

const QMatrix4x4 & Camera::viewInverted() const
{
    if (m_viewInvertedChanged)
    {
        m_viewInverted = view().inverted();
    }
    m_viewInvertedChanged = false;

    return m_viewInverted;
}

const QMatrix4x4 & Camera::projectionInverted() const
{
    if (m_projectionInvertedChanged)
    {
        m_projectionInverted = projection().inverted();
    }
    m_projectionInvertedChanged = false;

    return m_projectionInverted;
}

const QMatrix4x4 & Camera::viewProjectionInverted() const
{
    if (m_viewProjectionInvertedChanged)
    {
        m_viewProjectionInverted = viewProjection().inverted();
    }
    m_viewProjectionInvertedChanged = false;

    return m_viewProjectionInverted;
}

const QMatrix3x3 & Camera::normal() const
{
    if (m_normalChanged)
    {
        m_normal = view().normalMatrix();
    }
    m_normalChanged = false;

    return m_normal;
}

void Camera::update()
{
    if(m_lockedToTrain)
    {
        setEye(m_lockedObject->position() + m_lockedEyeOffset);
        setCenter(m_lockedObject->position() + m_lockedCenterOffset);
    }
}

void Camera::setMatrices(Program & program, const QMatrix4x4 & model) const
{
    program.setUniform(std::string("mModel"), model);
    program.setUniform(std::string("mModelInv"), model.inverted());
    program.setUniform(std::string("mModelNorm"), model.normalMatrix());
    program.setUniform(std::string("mView"), view());
    program.setUniform(std::string("mViewInv"), viewInverted());
    program.setUniform(std::string("mViewNorm"), normal());
    program.setUniform(std::string("mProjection"), projection());
    program.setUniform(std::string("mProjectionInv"), projectionInverted());
}
} // namespace terminus
