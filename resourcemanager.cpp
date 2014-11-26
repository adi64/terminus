#include "resourcemanager.h"

#include <fstream>
#include <string>
#include "geometry.h"

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{
    //TODO delete m_geometryStorage contents
}

Geometry ResourceManager::loadGeometry(std::string name, std::string path)
{
    std::vector<glm::vec3> indexBlocks;

    //first reading the obj file

    indexBlocks = parseObjFile(path);

    //now combining the 3 vectors into one vertex-buffer and generating an index-buffer for it

    std::vector<int> indexBuffer;
    std::vector<Geometry::Vertex> vertexBuffer;

    generateBuffers(indexBlocks, indexBuffer, vertexBuffer);

    Geometry tempGeo(indexBuffer, vertexBuffer);
    return tempGeo;
}

std::vector<glm::vec3> ResourceManager::parseObjFile(std::string path){

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> indexBlocks;

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
            glm::ivec3 indexA, indexB, indexC; //posIndex, texIndex, normIndex;
            line.replace('/', '');
            //std::stringstream localStream(line);
            lineStream >> indexA.x >> indexA.y >> indexA.z >> indexB.x >> indexB.y >> indexB.z >> indexC.x >> indexC.y >> indexC.z;
            indexBlocks.push_back(indexA);
            indexBlocks.push_back(indexB);
            indexBlocks.push_back(indexC);
        }
        else if(lineHeader == "mtllib")
        {
            std::string mtlFile;
            lineStream >> mtlFile;
            loadMaterial(mtlFile);
        }
        else if(lineHeader == "usemtl")
        {
            std::string mtlName;
            lineStream >> mtlName;
            //TODO use material;
        }
    }

    return indexBlocks;
}

void ResourceManager::generateBuffers(std::vector<glm::vec3> indexBlocks, std::vector<int> indexBuffer, std::vector<Geometry::Vertex> vertexBuffer)
{
    std::map<glm::ivec, int> indexLookUp;

    for(int i = 0; i < indexBlocks.size(); i++)
    {
        if(indexLookUp.count(indexBlocks[i]) == 0)
        {
            indexLookUp[indexBlocks[i]] = vertexBuffer.size();

            Geometry::Vertex v;
            v.position = indexBlocks[i].x;
            v.texCoord = indexBlocks[i].y;
            v.normal = indexBlocks[i].z;
            vertexBuffer.push_back(v);

            indexBuffer.push_back(vertexBuffer.size() - 1);
        }
        else
        {
            indexBuffer.push_back(indexLookUp[indexBlocks[i]]);
        }
    }
}

void ResourceManager::loadMaterial(std::string name, std::string path)
{
    //TODO: parse the .mtl file
}

std::unique_ptr<Geometry> ResourceManager::getGeometry(std::string name)
{
    if(m_geometryStorage.count(name) == 0)
        m_geometryStorage[name] = std::shared_ptr<std::unique_ptr<Geometry>>(new std::unique_ptr<Geometry>(new Geometry()));
    return m_geometryStorage[name];
}
