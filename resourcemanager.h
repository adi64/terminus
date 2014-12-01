#pragma once

#include <map>
#include <string>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "geometry.h"

namespace terminus
{

class ResourceManager
{
public:
    ResourceManager();
    virtual ~ResourceManager();

    virtual void storeGeometry(std::string name, std::string path);
    virtual Geometry loadGeometry(std::string path);
    virtual void loadMaterial(std::string path);
    virtual void ResourceManager::parseObjFile(std::string path,
                                               std::vector<glm::vec3> positions,
                                               std::vector<glm::vec3> texCoords,
                                               std::vector<glm::vec3> normals,
                                               std::vector<glm::ivec3> indexBlocks);
    virtual void generateBuffers(std::vector<glm::vec3> positions,
                                 std::vector<glm::vec3> texCoords,
                                 std::vector<glm::vec3> normals,
                                 std::vector<glm::ivec3> indexBlocks,
                                 std::vector<int> indexBuffer,
                                 std::vector<Vertex> vertexBuffer);

    virtual void ResourceManager::setGeometry(std::string name, Geometry * geometry);
    virtual std::shared_ptr<std::unique_ptr<Geometry>> getGeometry(std::string name);

protected:
    std::map<std::string, std::shared_ptr<std::unique_ptr<Geometry>>> m_geometryStorage;
};

} // terminus
