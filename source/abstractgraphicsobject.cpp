#include "abstractgraphicsobject.h"

#include <QDebug>
#include "scene.h"

namespace terminus
{

AbstractGraphicsObject::AbstractGraphicsObject(Scene *scene)
    : m_scene(scene)
    , m_position(0.0, 0.0, 0.0)
{
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

QMatrix4x4 AbstractGraphicsObject::modelMatrix() const
{
    return m_modelMatrix;
}

}
