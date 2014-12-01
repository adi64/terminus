#include "abstractgraphicsobject.h"

namespace terminus
{

AbstractGraphicsObject::AbstractGraphicsObject()
: m_worldPosition()
, m_rotation()
{
}


glm::vec3 AbstractGraphicsObject::getWorldPosition()
{
    return m_worldPosition;
}

glm::vec3 AbstractGraphicsObject::getRotation()
{
    return m_rotation;
}

void AbstractGraphicsObject::setWorldPosition(glm::vec3 newPosition)
{
    m_worldPosition = newPosition;
}

void AbstractGraphicsObject::setRotation(glm::vec3 newRotation)
{
    m_rotation = newRotation;
}

}
