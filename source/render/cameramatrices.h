#pragma once


#include <render/camerastate.h>

#include <QMatrix3x3>
#include <QMatrix4x4>


namespace terminus
{

class QPoint;
class QVector3D;

class CameraMatrices : public CameraState
{

public:
    CameraMatrices();

    virtual void setEye(const QVector3D & eye) override;
    virtual void setCenter(const QVector3D & center) override;
    virtual void setUp(const QVector3D & up) override;
    virtual void setZNear(float zNear) override;
    virtual void setZFar(float zFar) override;
    virtual void setFovy(float fovy) override;
    virtual void setViewport(const QPoint & viewport) override;

    const QMatrix4x4 & view() const;
    const QMatrix4x4 & projection() const;
    const QMatrix4x4 & viewProjection() const;
    const QMatrix4x4 & viewInverted() const;
    const QMatrix4x4 & projectionInverted() const;
    const QMatrix4x4 & viewProjectionInverted() const;
    const QMatrix3x3 & normal() const;

protected:
    void invalidateMatrices() const;

protected:
    mutable QMatrix4x4 m_view;
    mutable QMatrix4x4 m_viewInverted;
    mutable QMatrix4x4 m_projection;
    mutable QMatrix4x4 m_projectionInverted;
    mutable QMatrix4x4 m_viewProjection;
    mutable QMatrix4x4 m_viewProjectionInverted;
    mutable QMatrix3x3 m_normal;

    mutable bool m_viewChanged;
    mutable bool m_viewInvertedChanged;
    mutable bool m_projectionChanged;
    mutable bool m_projectionInvertedChanged;
    mutable bool m_viewProjectionChanged;
    mutable bool m_viewProjectionInvertedChanged;
    mutable bool m_normalChanged;
};

} // namespace terminus
