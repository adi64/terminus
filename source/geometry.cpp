#include "geometry.h"

#include <regex>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>

#include <QDebug>

#include "indextriple.h"

namespace terminus
{

Geometry * Geometry::loadObj(std::string path){
    std::vector<QVector3D> positions;
    std::vector<QVector3D> texCoords;
    std::vector<QVector3D> normals;
    std::vector<IndexTriple> indexTriples;

    std::vector<unsigned int> indexBuffer;
    std::vector<Vertex> vertexBuffer;

    std::vector<Geometry *> geometryList;

    //parse file and fill buffers with positions, texCoords, normals and index triples

    loadObjParse(path, positions, texCoords, normals, indexTriples);

    //use index triples to generate interleaved array buffer and single indices

    loadObjGenerate(positions, texCoords, normals, indexTriples, indexBuffer, vertexBuffer);

    return new Geometry(indexBuffer, vertexBuffer); //ownership is passed to caller
}
void Geometry::loadObjParse(std::string path,
                                   std::vector<QVector3D> & positions,
                                   std::vector<QVector3D> & texCoords,
                                   std::vector<QVector3D> & normals,
                                   std::vector<IndexTriple> & indexTriples){

    std::ifstream objFile(path);
    std::string line;

    while(std::getline(objFile, line))
    {
        std::string lineHeader;
        std::stringstream lineStream(line);
        lineStream >> lineHeader;

        if(lineHeader == "v")
        {
            float x,y,z;
            lineStream >> x >> y >> z;
            QVector3D position(x, y, z);
            positions.push_back(position);
        }
        else if(lineHeader == "vt")
        {
            float x,y,z;
            lineStream >> x >> y >> z;
            QVector3D texCoord(x, y, z);
            texCoords.push_back(texCoord);
        }
        else if(lineHeader == "vn")
        {
            float x,y,z;
            lineStream >> x >> y >> z;
            QVector3D normal(x, y, z);
            normals.push_back(normal);
        }
        else if(lineHeader == "f")
        {
            std::regex indexPattern("([0-9]+)(?:\\/([0-9]*)(?:\\/([0-9]*))?)?", std::regex::extended);
            std::string indexSpec[3];
            lineStream >> indexSpec[0] >> indexSpec[1] >> indexSpec[2];
            for(int i = 0; i < 3; i++)
            {
                std::smatch match;
                if(!std::regex_match(indexSpec[i], match, indexPattern))
                {
                    qDebug() << "FATAL: incorrect *.obj format";
                    return;
                }
                indexTriples.push_back(IndexTriple(match[1], match[2], match[3]));
            }
        }
        //materials
//        else if(lineHeader == "mtllib")
//        {
//            std::string mtlFile;
//            lineStream >> mtlFile;
//            //TODO loadMaterial(mtlFile);
//        }
//        else if(lineHeader == "usemtl")
//        {
//            std::string mtlName;
//            lineStream >> mtlName;
//            //TODO use material;
//        }
    }
}

void Geometry::loadObjGenerate(std::vector<QVector3D> & positions,
                                        std::vector<QVector3D> & texCoords,
                                        std::vector<QVector3D> & normals,
                                        std::vector<IndexTriple> & indexTriples,
                                        std::vector<unsigned int> & indexBuffer,
                                        std::vector<Vertex> & vertexBuffer)
{
    std::map<IndexTriple, unsigned int> indexLookUp;

    for(unsigned int i = 0; i < indexTriples.size(); i++)
    {
        if(indexLookUp.count(indexTriples[i]) == 0)
        {
            indexLookUp[indexTriples[i]] = vertexBuffer.size();

            Vertex v;
            v.position = positions[indexTriples[i].positionIndex()];
            v.texCoord = texCoords[indexTriples[i].textureIndex()];
            v.normal = normals[indexTriples[i].normalIndex()];
            vertexBuffer.push_back(v);

            indexBuffer.push_back(vertexBuffer.size() - 1);
        }
        else
        {
            indexBuffer.push_back(indexLookUp[indexTriples[i]]);
        }
    }
}
Geometry::Geometry()
{
}

Geometry::Geometry(const std::vector<unsigned int> & indexBuffer, const std::vector<Vertex> & vertexBuffer)
: m_indexBuffer(indexBuffer)
, m_vertexBuffer(vertexBuffer)
{
    //TODO allocate and fill VAO
}

Geometry::~Geometry()
{
    //TODO release GPU resources
}

}//namespace terminus
