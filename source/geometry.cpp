#include "geometry.h"

namespace terminus
{

Geometry::Geometry()
{
}

Geometry::Geometry(std::vector<int> indexBuffer, std::vector<Vertex> vertexBuffer)
: m_indexBuffer(indexBuffer)
, m_vertexBuffer(vertexBuffer)
{
}

} //terminus
