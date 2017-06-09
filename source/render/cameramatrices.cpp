#include "cameramatrices.h"

#include <cmath>

#include <QPoint>
#include <QVector3D>

namespace terminus
{

CameraMatrices::CameraMatrices()
{
}

void CameraMatrices::setEye(const QVector3D & eye)
{
    if (eye == m_eye)
    {
        return;
    }

    CameraState::setEye(eye);
    invalidateMatrices();
}

void CameraMatrices::setCenter(const QVector3D & center)
{
    if (center == m_center)
    {
        return;
    }

    CameraState::setCenter(center);
    invalidateMatrices();
}

void CameraMatrices::setUp(const QVector3D & up)
{
    if (up == m_up)
    {
        return;
    }

    CameraState::setUp(up);
    invalidateMatrices();
}

void CameraMatrices::setZNear(float zNear)
{
    if (fabs(zNear - m_zNear) < std::numeric_limits<float>::epsilon())
    {
        return;
    }

    CameraState::setZNear(zNear);
    invalidateMatrices();
}

void CameraMatrices::setZFar(float zFar)
{
    if (fabs(zFar - m_zFar) < std::numeric_limits<float>::epsilon())
    {
        return;
    }

    CameraState::setZFar(zFar);
    invalidateMatrices();
}

void CameraMatrices::setFovy(float fovy)
{
    if (fabs(fovy - m_fovy) < std::numeric_limits<float>::epsilon())
    {
        return;
    }

    CameraState::setFovy(fovy);
    invalidateMatrices();
}

void CameraMatrices::setViewport(const QPoint & viewport)
{
    if (viewport == m_viewport)
    {
        return;
    }

    CameraState::setViewport(viewport);
    invalidateMatrices();
}

void CameraMatrices::setViewport(int width, int height)
{
    setViewport(QPoint(width, height));
}

void CameraMatrices::invalidateMatrices() const
{
    m_viewChanged = true;
    m_viewInvertedChanged = true;
    m_projectionChanged = true;
    m_projectionInvertedChanged = true;
    m_viewProjectionChanged = true;
    m_viewProjectionInvertedChanged = true;
    m_normalChanged = true;
}

const QMatrix4x4 & CameraMatrices::view() const
{
    if (m_viewChanged)
    {
        m_view.setToIdentity();
        m_view.lookAt(m_eye, m_center, m_up);
    }
    m_viewChanged = false;

    return m_view;
}

const QMatrix4x4 & CameraMatrices::projection() const
{
    if (m_projectionChanged)
    {
        m_projection.setToIdentity();
        m_projection.perspective(m_fovy, m_aspect, m_zNear, m_zFar);
    }
    m_projectionChanged = false;

    return m_projection;
}

const QMatrix4x4 & CameraMatrices::viewProjection() const
{
    if (m_viewProjectionChanged)
    {
        m_viewProjection = projection() * view();
    }
    m_viewProjectionChanged = false;

    return m_viewProjection;
}

const QMatrix4x4 & CameraMatrices::viewInverted() const
{
    if (m_viewInvertedChanged)
    {
        m_viewInverted = view().inverted();
    }
    m_viewInvertedChanged = false;

    return m_viewInverted;
}

const QMatrix4x4 & CameraMatrices::projectionInverted() const
{
    if (m_projectionInvertedChanged)
    {
        m_projectionInverted = projection().inverted();
    }
    m_projectionInvertedChanged = false;

    return m_projectionInverted;
}

const QMatrix4x4 & CameraMatrices::viewProjectionInverted() const
{
    if (m_viewProjectionInvertedChanged)
    {
        m_viewProjectionInverted = viewProjection().inverted();
    }
    m_viewProjectionInvertedChanged = false;

    return m_viewProjectionInverted;
}

const QMatrix3x3 & CameraMatrices::normal() const
{
    if (m_normalChanged)
    {
        m_normal = view().normalMatrix();
    }
    m_normalChanged = false;

    return m_normal;
}

} // namespace terminus
