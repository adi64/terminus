#pragma once

#include <QMatrix3x3>
#include <QMatrix4x4>
#include <QPoint>
#include <QVector3D>
#include <QVector2D>

#include <resources/program.h>

namespace terminus
{

class GameObject;

/*!
 * \brief The Camera class manages view and projection matrices that define
 * a view of a 3D scene.
 *
 * It also contains a mechanism that allows it to be bound to
 * GameObjects in order to track their movement and
 * implements a simple navigation mechanism that is active if it is not bound.
 *
 * \sa GameObject
 */
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

    QVector3D normalizedLook() const;

    float zNear() const;
    void setZNear(float zNear);
    float zFar() const;
    void setZFar(float zFar);

    float fovy() const;
    void setFovy(float fovy);

    const QPoint & viewport() const;
    void setViewport(const QPoint & viewport);
    void setViewport(int width, int height);

    float aspectRatio() const;

    const QMatrix4x4 & view() const;
    const QMatrix4x4 & projection() const;
    const QMatrix4x4 & viewProjection() const;
    const QMatrix4x4 & viewInverted() const;
    const QMatrix4x4 & projectionInverted() const;
    const QMatrix4x4 & viewProjectionInverted() const;
    const QMatrix3x3 & normal() const;

    /*!
     * \brief sets the view and projection as well as the given model matrix
     * as uniforms on the given shader program
     * \param program
     * \param model
     */
    void setUniforms(Program & program, const QMatrix4x4 & model) const;

    /*!
     * \brief bind camera to track an GameObject
     * \param object
     *
     * passing a nullptr releases the camera
     */
    void bindTo(GameObject * object);
    /*!
     * \brief release the camera from the object it is currently bound to
     */
    void unbind();
    /*!
     * \brief is called if an object releases this camera
     * \param object
     */
    void unbound(GameObject * object);
    /*!
     * \return true if this camera is currently bound to track an
     * GameObject
     */
    bool isBound() const;

    /*!
     * \brief called if the GUI requests the camera to be moved
     * \param movement
     *
     * If this camera is bound to an object, this object's moveEvent() method
     * will be called.
     * Otherwise the camera is moved according to a simple navigation mechanism.
     */
    void moveEvent(QVector3D movement);
    /*!
     * \brief called if the GUI requests the camera to be rotated
     * \param rotation
     *
     * If this camera is bound to an object, this object's rotateEvent() method
     * will be called.
     * Otherwise the camera is rotated according to a simple navigation mechanism.
     */
    void rotateEvent(QVector2D rotation);

protected:
    void invalidateMatrices() const;

protected:
    QVector3D m_eye;
    QVector3D m_center;
    QVector3D m_up;

    float m_fovy;
    float m_aspect;
    float m_zNear;
    float m_zFar;

    QPoint m_viewport;

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

    /*!
     * \brief object that this camera is bound to in order to track it
     */
    GameObject * m_associatedObject;
};

} // namespace terminus
