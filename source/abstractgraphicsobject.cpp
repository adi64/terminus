#include "abstractgraphicsobject.h"

#include <QDebug>

#include <bullet/btBulletDynamicsCommon.h>

#include "scene.h"

namespace terminus
{

AbstractGraphicsObject::AbstractGraphicsObject(Scene *scene)
    : m_scene(scene)
    , m_position(0.0, 0.0, 0.0)
    , m_eulerAngles(0.0, 0.0, 0.0)
    , m_scaling(1.0, 1.0, 1.0)
{
}

void AbstractGraphicsObject::update(int elapsedMilliseconds)
{
    auto angles = eulerAngles();

    m_modelMatrix.setToIdentity();
    m_modelMatrix.translate(position());
    m_modelMatrix.rotate(angles.x(), angles.y(), angles.z());
    m_modelMatrix.scale(scaling());
}

qreal AbstractGraphicsObject::calculateDistance() const
{
    return (m_scene->camera().eye() - m_position).length(); //just the dirty way, has to be improved
}

void AbstractGraphicsObject::setPosition(const QVector3D &newPosition)
{
    m_position = newPosition;
}

QVector3D AbstractGraphicsObject::position() const
{
    return m_position;
}

QVector3D AbstractGraphicsObject::eulerAngles() const
{
    return m_eulerAngles;
}

QVector3D AbstractGraphicsObject::scaling() const
{
    return m_scaling;
}

QMatrix4x4 AbstractGraphicsObject::modelMatrix() const
{
    return m_modelMatrix;
}

bool AbstractGraphicsObject::canCollide() const
{
    return true;
}

}
