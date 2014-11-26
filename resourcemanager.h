#pragma once

#include <map>
#include <string>
#include <memory>
#include <glm/glm.hpp>

class Geometry;

class ResourceManager
{
public:
    ResourceManager();
    virtual ~ResourceManager();

    virtual Geometry loadGeometry(std::string name, std::string path);
    virtual void loadMaterial(std::string name, std::string path);
    virtual std::vector<glm::vec3> ResourceManager::parseObjFile(std::string path);
    virtual void generateBuffers(std::vector<glm::vec3> indexBlocks, std::vector<int> indexBuffer, std::vector<Geometry::Vertex> vertexBuffer);

    virtual std::shared_ptr<std::unique_ptr<Geometry>> getGeometry(std::string name);

protected:
    std::map<std::string, std::shared_ptr<std::unique_ptr<Geometry>>> m_geometryStorage;
};

