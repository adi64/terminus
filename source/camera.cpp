#include "camera.h"
#include <cassert>
#include <QQuaternion>

namespace terminus
{

Camera::Camera(
    const QVector3D & eye
,   const QVector3D & center
,   const QVector3D & up)
: m_dirty(false)

, m_eye(eye)
, m_center(center)
, m_up(up)

, m_fovy(90.f) // degrees!
, m_aspect(16.f / 9.f)
, m_zNear(0.2f)
, m_zFar(128.0f)
, m_viewChanged(true)
, m_viewInvertedChanged(true)
, m_projectionChanged(true)
, m_projectionInvertedChanged(true)
, m_viewProjectionChanged(true)
, m_viewProjectionInvertedChanged(true)
, m_normalChanged(true)
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

void Camera::dirty()
{
    m_dirty = true;

    this->update();
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
    dirty();
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
    dirty();
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
    dirty();
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

    dirty();
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

    dirty();
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

    dirty();
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

    dirty();
}

float Camera::aspectRatio() const
{
    return m_aspect;
}

void Camera::update() const
{
    if (!m_dirty)
    {
        return;
    }

    // todo fixme - dirty is set to false here because otherwise invalidateMatrices() would call this in a loop
    m_dirty = false;

    invalidateMatrices();
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

void Camera::setRotation(QVector2D rotation)
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

const QMatrix4x4 & Camera::view() const
{
    if (m_dirty)
    {
        update();
    }

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
    if (m_dirty)
    {
        update();
    }

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
    if (m_dirty)
    {
        update();
    }

    if (m_viewProjectionChanged)
    {
        m_viewProjection = projection() * view();
    }

    m_viewProjectionChanged = false;

    return m_viewProjection;
}

const QMatrix4x4 & Camera::viewInverted() const
{
    if (m_dirty)
    {
        update();
    }

    if (m_viewInvertedChanged)
    {
        m_viewInverted = view().inverted();
    }

    m_viewInvertedChanged = false;

    return m_viewInverted;
}

const QMatrix4x4 & Camera::projectionInverted() const
{
    if (m_dirty)
    {
        update();
    }

    if (m_projectionInvertedChanged)
    {
        m_projectionInverted = projection().inverted();
    }

    m_projectionInvertedChanged = false;

    return m_projectionInverted;
}

const QMatrix4x4 & Camera::viewProjectionInverted() const
{
    if (m_dirty)
    {
        update();
    }

    if (m_viewProjectionInvertedChanged)
    {
        m_viewProjectionInverted = viewProjection().inverted();
    }

    m_viewProjectionInvertedChanged = false;

    return m_viewProjectionInverted;
}

const QMatrix3x3 & Camera::normal() const
{
    if (m_dirty)
    {
        update();
    }

    if (m_normalChanged)
    {
        m_normal = view().normalMatrix();
    }

    m_normalChanged = false;

    return m_normal;
}


} // namespace terminus
