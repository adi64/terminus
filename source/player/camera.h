#pragma once


#include <QPoint>
#include <QVector3D>
#include <QVector2D>

#include <resources/program.h>
#include <render/cameramatrices.h>

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
    Camera();
    virtual ~Camera();

    const CameraMatrices & state() const;
    CameraMatrices & state();

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
    CameraMatrices m_state;

    /*!
     * \brief object that this camera is bound to in order to track it
     */
    GameObject * m_associatedObject;
};

} // namespace terminus
