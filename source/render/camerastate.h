#pragma once


#include <QPoint>
#include <QVector3D>


namespace terminus
{

class CameraState
{
public:
    CameraState(const QVector3D & eye    = QVector3D(0.0, 0.0, 1.0),
                const QVector3D & center = QVector3D(0.0, 0.0, 0.0),
                const QVector3D & up     = QVector3D(0.0, 1.0, 0.0) );
    virtual ~CameraState();

    virtual const QVector3D & eye() const;
    virtual void setEye(const QVector3D & eye);
    virtual const QVector3D & center() const;
    virtual void setCenter(const QVector3D & center);
    virtual const QVector3D & up() const;
    virtual void setUp(const QVector3D & up);

    virtual QVector3D normalizedLook() const;

    virtual float zNear() const;
    virtual void setZNear(float zNear);
    virtual float zFar() const;
    virtual void setZFar(float zFar);

    virtual float fovy() const;
    virtual void setFovy(float fovy);

    virtual const QPoint & viewport() const;
    virtual void setViewport(const QPoint & viewport);
    virtual void setViewport(int width, int height);

    virtual float aspectRatio() const;

protected:
    QVector3D m_eye;
    QVector3D m_center;
    QVector3D m_up;

    float m_fovy;
    float m_aspect;
    float m_zNear;
    float m_zFar;

    QPoint m_viewport;

};

} // namespace terminus

