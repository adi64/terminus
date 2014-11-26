#pragma once

#include <glm/glm.hpp>

struct {
    glm::vec3 position;
    glm::vec3 texCoord;
    glm::vec3 normal;
    //TODO find ALL relevant fields
} Vertex;

class Geometry
{
public:
    Geometry();//construct NULL-Object
    Geometry(std::vector<int> indexBuffer, std::vector<Vertex> vertexBuffer);    //TODO define constructor taking vertex and index buffers, material refs?

protected:
    std::vector<Vertex> m_vertexBuffer;
    std::vector<int> m_indexBuffer;
};

