#include "geometry.h"

#include <regex>
#include <fstream>
#include <string>
#include <sstream>

namespace terminus
{


static Geometry * Geometry::loadObj(std::string path){
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<glm::ivec3> indexBlocks;

    std::vector<unsigned int> indexBuffer;
    std::vector<Vertex> vertexBuffer;

    //parse file and fill buffers with positions, texCoords, normals and index triples

    parseObjFile(path, positions, texCoords, normals, indexBlocks);

    //use index triples to generate interleaved array buffer and single indices

    generateBuffers(positions, texCoords, normals, indexBlocks, indexBuffer, vertexBuffer);

    return new Geometry(indexBuffer, vertexBuffer); //ownership is passed to caller
}

static void Geometry::loadObjParse(std::string path,
                                   std::vector<glm::vec3> & positions,
                                   std::vector<glm::vec3> & texCoords,
                                   std::vector<glm::vec3> & normals,
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
            glm::vec3 position;
            lineStream >> position.x >> position.y >> position.z;
            positions.push_back(position);
        }
        else if(lineHeader == "vt")
        {
            glm::vec3 texCoord;
            lineStream >> texCoord.x >> texCoord.y >> texCoord.z;
            texCoords.push_back(texCoord);
        }
        else if(lineHeader == "vn")
        {
            glm::vec3 normal;
            lineStream >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if(lineHeader == "f")
        {
            std::regex indexPattern("([0-9]+)/([0-9]*)/([0-9]*)", std::regex::extended);
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
        else if(lineHeader == "mtllib")
        {
            std::string mtlFile;
            lineStream >> mtlFile;
            //TODO loadMaterial(mtlFile);
        }
        else if(lineHeader == "usemtl")
        {
            std::string mtlName;
            lineStream >> mtlName;
            //TODO use material;
        }
    }
}

static void Geometry::loadObjGenerate(std::vector<glm::vec3> & positions,
                                      std::vector<glm::vec3> & texCoords,
                                      std::vector<glm::vec3> & normals,
                                      std::vector<IndexTriple> & indexBlocks,
                                      std::vector<unsigned int> & indexBuffer,
                                      std::vector<Vertex> & vertexBuffer)
{
    std::unordered_map<glm::ivec3, unsigned int> indexLookUp; //define less function on glm::ivec3

    for(unsigned int i = 0; i < indexBlocks.size(); i++)
    {
        if(indexLookUp.count(&(indexBlocks[i])) == 0)
        {
            indexLookUp[&(indexBlocks[i])] = vertexBuffer.size();

            Vertex v;
            v.position = positions[indexBlocks[i].x];
            v.texCoord = texCoords[indexBlocks[i].y];
            v.normal = normals[indexBlocks[i].z];
            vertexBuffer.push_back(v);

            indexBuffer.push_back(vertexBuffer.size() - 1);
        }
        else
        {
            indexBuffer.push_back(indexLookUp[&(indexBlocks[i])]);
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

} //terminus
