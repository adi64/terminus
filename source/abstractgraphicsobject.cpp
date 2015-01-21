#include "abstractgraphicsobject.h"

#include <QQuaternion>
#include <QDebug>

#include "scene.h"

namespace terminus
{

AbstractGraphicsObject::AbstractGraphicsObject(std::shared_ptr<Scene> scene)
: m_scene(scene)
, m_position(0.0, 0.0, 0.0)
, m_rotation(1.0, 0.0, 0.0, 0.0)
, m_scale(1.0, 1.0, 1.0)
, m_modelMatrixChanged(false)
{
}

AbstractGraphicsObject::~AbstractGraphicsObject()
{
    // do not delete this destructor, even if it is empty
    // otherwise std::shared_ptr<IncompleteType> in the header will break
    //
    // ... :D
}

void AbstractGraphicsObject::update(int elapsedMilliseconds)
{
}

QVector3D AbstractGraphicsObject::position() const
{
    return m_position;
}
QQuaternion AbstractGraphicsObject::rotation() const
{
    return m_rotation;
}
QVector3D AbstractGraphicsObject::scale() const
{
    return m_scale;
}

QMatrix4x4 AbstractGraphicsObject::modelMatrix() const
{
    if(m_modelMatrixChanged)
    {
        m_modelMatrix.setToIdentity();
        m_modelMatrix.translate(position());
        m_modelMatrix.rotate(rotation());
        m_modelMatrix.scale(scale());
        m_modelMatrixChanged = false;
    }
    return m_modelMatrix;
}

void AbstractGraphicsObject::setPosition(const QVector3D & position)
{
    m_position = position;
    m_modelMatrixChanged = true;
}
void AbstractGraphicsObject::setRotation(const QQuaternion &eulerAngles)
{
    m_rotation = eulerAngles;
    m_modelMatrixChanged = true;
}
void AbstractGraphicsObject::setScale(const QVector3D & scale)
{
    m_scale = scale;
    m_modelMatrixChanged = true;
}
void AbstractGraphicsObject::setScale(float scale)
{
    m_scale = QVector3D(scale, scale, scale);
    m_modelMatrixChanged = true;
}

}
