#pragma once

#include <glm/glm.hpp>

namespace terminus
{

class AbstractGraphicsObject
{
public:
    AbstractGraphicsObject();
    virtual void render() = 0;

    glm::vec3 getWorldPosition();
    glm::vec3 getRotation();

    void setWorldPosition(glm::vec3 newPosition);
    void setRotation(glm::vec3 newRotation);

protected:
    glm::vec3 m_worldPosition;
    glm::vec3 m_rotation;
};

}
