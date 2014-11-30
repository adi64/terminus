#include "geometry.h"

Geometry::Geometry()
{
}

Geometry::Geometry(std::vector<int> indexBuffer, std::vector<GeoVertex::Vertex> vertexBuffer)
: m_indexBuffer(indexBuffer)
, m_vertexBuffer(vertexBuffer)
{
}
