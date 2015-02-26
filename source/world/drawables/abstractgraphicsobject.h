#pragma once

#include <memory>
#include <functional>

#include <QOpenGLFunctions>
#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

#include <world/world.h>
#include <resources/geometry.h>
#include <resources/material.h>
#include <resources/program.h>

namespace terminus
{

class AbstractGraphicsObject
{
public:
    AbstractGraphicsObject(World & world);
    virtual ~AbstractGraphicsObject();

    /*!
     * \brief AbstractGraphicsObject::update - update object hierarchy beginning at this object
     * \param elapsedMilliseconds
     *
     * In general there is no need to override this.
     * \sa AbstractGraphicsObject::localUpdate
     */
    virtual void update(int elapsedMilliseconds) final;

    /*!
     * \brief AbstractGraphicsObject::render - render object hierarchy beginning at this object
     * \param gl - GL functions
     *
     * In general there is no need to override this.
     * \sa AbstractGraphicsObject::localRender
     */
    virtual void render(QOpenGLFunctions & gl) final;

    virtual QVector3D worldUp();
    virtual QVector3D worldFront();
    virtual QVector3D worldRight();

    virtual QVector3D position() const;
    virtual QQuaternion rotation() const;
    virtual QVector3D scale() const;
    virtual QMatrix4x4 modelMatrix() const;

protected:
    /*!
     * \brief AbstractGraphicsObject::localUpdate - update the state of this object (as yet nothing to do)
     */
    virtual void localUpdate(int elapsedMilliseconds);

    /*!
     * \brief AbstractGraphicsObject::localRender - render this object using geometry, program and material set in the respective members
     * \param gl - GL functions
     *
     * Overrrite this if custom render code is needed
     */
    virtual void localRender(QOpenGLFunctions & gl) const;

    /*!
     * \brief AbstractGraphicsObject::localRenderSetup - use this to configure additional render states
     * \param gl - GL functions
     * \param program - program bound for rendering
     *
     * Set object specific uniforms, textures &c here
     */
    virtual void localRenderSetup(QOpenGLFunctions & gl, Program & program) const;

    /*!
     * \brief AbstractGraphicsObject::localRenderCleanup - use this to clean up render states set in AbstractGraphicsObject::localRenderSetup
     * \param gl - GL functions
     * \param program - program bound for rendering
     */
    virtual void localRenderCleanup(QOpenGLFunctions & gl, Program & program) const;

    virtual bool localRenderEnabled() const;

    virtual void setPosition(const QVector3D & position);
    virtual void setRotation(const QQuaternion & eulerAngles);
    virtual void setScale(const QVector3D & scale);
    virtual void setScale(float scale);

    /*!
     * \brief doForAllChildren - calls callback for every child object
     * \param callback
     *
     * subclasses with children must override this method accordingly
     */
    virtual void doForAllChildren(std::function<void(AbstractGraphicsObject &)> callback);

protected:
    World & m_world;

    std::shared_ptr<std::unique_ptr<Program>> m_program;
    std::shared_ptr<std::unique_ptr<Geometry>> m_geometry;
    std::shared_ptr<std::unique_ptr<Material>> m_material;

    QVector3D m_position;
    QQuaternion m_rotation;
    QVector3D m_scale;
    mutable bool m_modelMatrixChanged;
    mutable QMatrix4x4 m_modelMatrix;
};

}
