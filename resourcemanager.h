#pragma once

#include <map>
#include <string>
#include <memory>

class Geometry;

class ResourceManager
{
public:
    ResourceManager();
    virtual ~ResourceManager();

    virtual void loadGeometry(std::string name, std::string path);
    virtual std::shared_ptr<std::unique_ptr<Geometry>> getGeometry(std::string name);

protected:
    std::map<std::string, std::shared_ptr<std::unique_ptr<Geometry>>> m_geometryStorage;
};

