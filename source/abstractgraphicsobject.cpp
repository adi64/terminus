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

qreal AbstractGraphicsObject::calculateDistance()
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

}
