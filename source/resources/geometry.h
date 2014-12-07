#pragma once

#include <string>
#include <vector>

#include <QVector3D>

#include "indextriple.h"

namespace terminus
{

struct Vertex{
    QVector3D position;
    QVector3D texCoord;
    QVector3D normal;
};

class Geometry
{
public:
    static Geometry * loadObj(std::string name);
protected:
    static void loadObjParse(std::string path,
                                        std::vector<QVector3D> & positions,
                                        std::vector<QVector3D> & texCoords,
                                        std::vector<QVector3D> & normals,
                                        std::vector<IndexTriple> & indexTriples);
    static void loadObjGenerate(std::vector<QVector3D> & positions,
                                            std::vector<QVector3D> & texCoords,
                                            std::vector<QVector3D> & normals,
                                            std::vector<IndexTriple> & indexTriples,
                                            std::vector<unsigned int> & indexBuffer,
                                            std::vector<Vertex> & vertexBuffer);

protected:
    Geometry(); //construct NULL-Object --- what do we need this for?
    Geometry(const std::vector<unsigned int> & indexBuffer, const std::vector<Vertex> & vertexBuffer);

public:
    ~Geometry();

protected:
    std::vector<Vertex> m_vertexBuffer;
    std::vector<unsigned int> m_indexBuffer;
};

} //terminus
