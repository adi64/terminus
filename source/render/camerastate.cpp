#include "camerastate.h"

#include <cassert>


namespace terminus
{

CameraState::CameraState(const QVector3D & eye, const QVector3D & center, const QVector3D & up)
: m_eye(eye)
, m_center(center)
, m_up(up)
, m_fovy(70.f) // degrees!
, m_aspect(16.f / 9.f)
, m_zNear(0.2f)
, m_zFar(1024.0f)
{
}

const QVector3D & CameraState::eye() const
{
    return m_eye;
}

void CameraState::setEye(const QVector3D & eye)
{
    m_eye = eye;
}

const QVector3D & CameraState::center() const
{
    return m_center;
}

void CameraState::setCenter(const QVector3D & center)
{
    m_center = center;
}

const QVector3D & CameraState::up() const
{
    return m_up;
}

void CameraState::setUp(const QVector3D & up)
{
    m_up = up;
}

QVector3D CameraState::normalizedLook() const
{
    return (center() - eye()).normalized();
}

float CameraState::zNear() const
{
    return m_zNear;
}

void CameraState::setZNear(const float zNear)
{
    m_zNear = zNear;
    assert(m_zNear > 0.0);
}

float CameraState::zFar() const
{
    return m_zFar;
}

void CameraState::setZFar(const float zFar)
{
    m_zFar = zFar;
    assert(m_zFar > m_zNear);
}

float CameraState::fovy() const
{
    return m_fovy;
}

void CameraState::setFovy(const float fovy)
{
    m_fovy = fovy;
    assert(m_fovy > 0.0);
}

const QPoint & CameraState::viewport() const
{
    return m_viewport;
}

void CameraState::setViewport(int width, int height)
{
    setViewport(QPoint(width, height));
}

void CameraState::setViewport(const QPoint & viewport)
{
    m_aspect = static_cast<float>(viewport.x()) / std::max(static_cast<float>(viewport.y()), 1.f);
    m_viewport = viewport;
}

float CameraState::aspectRatio() const
{
    return m_aspect;
}

} // namespace terminus

