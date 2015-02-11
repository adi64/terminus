#pragma once

#include <QVector3D>
#include <QVector2D>
#include <QMatrix3x3>
#include <QMatrix4x4>

#include "../resources/program.h"

namespace terminus
{

class AbstractGraphicsObject;

class Camera
{


public:
    Camera(const QVector3D & eye    = QVector3D(0.0, 0.0, 1.0),
           const QVector3D & center = QVector3D(0.0, 0.0, 0.0),
           const QVector3D & up     = QVector3D(0.0, 1.0, 0.0) );

    virtual ~Camera();

    const QVector3D & eye() const;
    void setEye(const QVector3D & eye);
    const QVector3D & center() const;
    void setCenter(const QVector3D & center);
    const QVector3D & up() const;
    void setUp(const QVector3D & up);

    float zNear() const;
    void setZNear(float zNear);
    float zFar() const;
    void setZFar(float zFar);

    float fovy() const;
    void setFovy(float fovy);

    const QVector2D & viewport() const; // should be int, but whatever
    void setViewport(const QVector2D & viewport);
    void setViewport(int width, int height);

    float aspectRatio() const;

    void setMatrices(Program & program, const QMatrix4x4 & model) const;

    const QMatrix4x4 & view() const;
    const QMatrix4x4 & projection() const;
    const QMatrix4x4 & viewProjection() const;
    const QMatrix4x4 & viewInverted() const;
    const QMatrix4x4 & projectionInverted() const;
    const QMatrix4x4 & viewProjectionInverted() const;
    const QMatrix3x3 & normal() const;

    void changed();
    void update();

    QVector3D movement();
    QVector2D rotation();

    void setMovement(QVector3D movement);
    void setRotation(QVector2D rotation);

    void toggleLocked();
    void setLocked(bool value);
    bool isLocked() const;
    void lockToObject(AbstractGraphicsObject *object);

protected:
    void invalidateMatrices() const;

protected:
    QVector3D m_movement;
    QVector2D m_rotation;
    bool m_lockedToTrain;

    QVector3D m_eye;
    QVector3D m_center;
    QVector3D m_up;

    float m_fovy;
    float m_aspect;
    float m_zNear;
    float m_zFar;

    QVector2D m_viewport;

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

    QVector3D m_lockedCenterOffset;
    QVector3D m_lockedFlickOffset;
    QQuaternion m_lockedEyeAngle;
    AbstractGraphicsObject *m_lockedObject;
};


} // namespace terminus
