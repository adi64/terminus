#include "abstractgraphicsobject.h"

namespace terminus
{

AbstractGraphicsObject::AbstractGraphicsObject()
: m_worldPosition()
, m_rotation()
{
}


QVector3D AbstractGraphicsObject::getWorldPosition()
{
    return m_worldPosition;
}

QVector3D AbstractGraphicsObject::getRotation()
{
    return m_rotation;
}

void AbstractGraphicsObject::setWorldPosition(QVector3D newPosition)
{
    m_worldPosition = newPosition;
}

void AbstractGraphicsObject::setRotation(QVector3D newRotation)
{
    m_rotation = newRotation;
}

}
