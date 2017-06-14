#pragma once

#include <memory>
#include <functional>

//#include <glincl.h>

#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

#include <resources/geometry.h>
#include <resources/material.h>
#include <resources/program.h>

namespace terminus
{

class Camera;
class Program;
class World;

/*!
 * \brief The AbstractGraphicsObject class is the root of a class hierarchy of
 * objects that contain independen game logic and are visible in the game.
 */
class AbstractGraphicsObject
{
public:
    AbstractGraphicsObject(World & world);
    /*!
     * Do not delete this destructor, even if it is empty
     *  - otherwise std::shared_ptr<IncompleteType> in the header will break
     */
    virtual ~AbstractGraphicsObject();

    /*!
     * \brief update object hierarchy beginning at this object
     *
     * This method is not meant to be overridden as it contains logic to
     * update child objects.
     *
     * \sa AbstractGraphicsObject::localUpdate
     */
    virtual void update() final;

    /*!
     * \brief render object hierarchy beginning at this object
     *
     * This method is not meant to be overridden as it contains logic to
     * update child objects.
     *
     * \sa AbstractGraphicsObject::localRender
     */
    virtual void render() final;

    /*!
     * \brief binds a camera to track this object
     * \param cam
     *
     * Once a camera has been bound, it passes move and rotate events on to this
     * object and adjustCamera() will be called from update()
     *
     * \sa adjustCamera()
     * \sa moveEvent()
     * \sa rotateEvent()
     */
    virtual void bindCamera(Camera * camera);
    /*!
     * \brief release a camera so that it wont track this object any longer
     *
     * \sa bindCamera()
     */
    virtual void unbindCamera(Camera * camera);

    /*!
     * \brief override this method to perform specific actions once a camera
     * has been bound
     */
    virtual void onBindCamera();
    /*!
     * \brief override this method to perform specific actions once a camera
     * has been released
     */
    virtual void onUnbindCamera();
    /*!
     * \brief override this method to adjust the parameters of a bound camera
     * according to this objects own state
     */
    virtual void adjustCamera();
    /*!
     * \brief override this method to react to move events received
     * by a bound camera
     * \param movement
     */
    virtual void moveEvent(QVector3D movement);
    /*!
     * \brief override this method to react to rotate events received
     * by a bound camera
     * \param rotation
     */
    virtual void rotateEvent(QVector2D rotation);

    /*!
     * \return the minimum vector of this objects AABB given that it has
     * a an associated Geometry instance
     */
    const QVector3D & minBB() const;
    /*!
     * \return the maximum vector of this objects AABB given that it has
     * a an associated Geometry instance
     */
    const QVector3D & maxBB() const;

    /*!
     * \return the up-vector of the model coordinate system transformed
     * to world space
     */
    QVector3D worldUp();
    /*!
     * \return the front-vector of the model coordinate system transformed
     * to world space
     */
    QVector3D worldFront();
    /*!
     * \return the side-vector of the model coordinate system transformed
     * to world space
     */
    QVector3D worldSide();

    /*!
     * \return the position vector that was used to construct the model matrix
     */
    QVector3D position() const;
    /*!
     * \return the quaternion that was used to construct the model matrix
     */
    QQuaternion rotation() const;
    /*!
     * \return the scaling vector that was used to construct the model matrix
     */
    QVector3D scale() const;

    /*!
     * \return this objects model matrix
     *
     * If necessary, the matrix is recalculated from the
     * position, rotation and scaling information
     */
    QMatrix4x4 modelMatrix() const;
    /*!
     * \return this objects inverted model matrix
     *
     * If necessary, the matrix is recalculated from the model matrix
     */
    QMatrix4x4 modelMatrixInverted() const;

protected:
    /*!
     * \brief override this method to update this objects state
     */
    virtual void localUpdate();

    /*!
     * \brief this method contains the default render code using #m_geometry,
     * #m_program and #m_material
     *
     * localRenderSetup() and localRenderCleanup are called from here and are
     * the preferred way to customize the rendering process over overriding
     * this method entirely
     */
    virtual void localRender() const;
    /*!
     * \brief override this method to customize the rendering process
     * \param program - already bound
     *
     * Here is the right place to set additional uniforms
     * and to bind textures &c.
     */
    virtual void localRenderSetup(Program & program) const;
    /*!
     * \brief override this method to customize the rendering process
     * \param program - still bound
     *
     * Here is the right place to clean up rendering states
     * set in localRenderSetup().
     */
    virtual void localRenderCleanup(Program & program) const;
    /*!
     * \brief override this method to disable or enable rendering conditionally
     * \return true if localRender() shall be called
     */
    virtual bool localRenderEnabled() const;

    /*!
     * \brief override this method to make child objects known
     * \param callback
     *
     * callback must be called once for every child object
     */
    virtual void doForAllChildren(std::function<void(AbstractGraphicsObject &)> callback);

    /*!
     * \brief sets the position and invalidates the model matrices
     * \param position
     */
    void setPosition(const QVector3D & position);
    /*!
     * \brief sets the rotation and invalidates the model matrices
     * \param rotation
     */
    void setRotation(const QQuaternion & rotation);
    /*!
     * \brief sets the scale and invalidates the model matrices
     * \param scale
     */
    void setScale(const QVector3D & scale);
    /*!
     * \brief sets the scale uniformly on all three axes
     * and invalidates the model matrices
     * \param scale
     */
    void setScale(float scale);

    /*!
     * \param vWorld
     * \return the model space position corresponding to vWorld in world space
     *
     * \sa modelMatrixInverted()
     */
    QVector3D worldToModel(const QVector3D & vWorld);
    /*!
     * \param vModel
     * \return the world space position corresponding to vModel in model space
     *
     * \sa modelMatrix()
     */
    QVector3D modelToWorld(const QVector3D & vModel);

    /*!
     * \brief remove this object from the world and delete it
     * as soon as it is safe to do so
     *
     * \sa World::deleteObject()
     */
    void dispose();

protected:
    World & m_world;
    bool m_validState;

    Camera * m_camera;

    std::shared_ptr<std::unique_ptr<Program>> m_program;
    std::shared_ptr<std::unique_ptr<Geometry>> m_geometry;
    std::shared_ptr<std::unique_ptr<Material>> m_material;

    QVector3D m_position;
    QQuaternion m_rotation;
    QVector3D m_scale;
    mutable bool m_modelMatrixChanged;
    mutable QMatrix4x4 m_modelMatrix;
    mutable bool m_modelMatrixInvertedChanged;
    mutable QMatrix4x4 m_modelMatrixInverted;
};

}
