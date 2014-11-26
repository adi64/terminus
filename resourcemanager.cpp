#include "resourcemanager.h"

#include "geometry.h"

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{
    //TODO delete m_geometryStorage contents
}

void ResourceManager::loadGeometry(std::string name, std::string path)
{
    //TODO implement .obj-Loader
}

std::unique_ptr<Geometry> ResourceManager::getGeometry(std::string name)
{
    if(m_geometryStorage.count(name) == 0)
        m_geometryStorage[name] = std::shared_ptr<std::unique_ptr<Geometry>>(new std::unique_ptr<Geometry>(new Geometry()));
    return m_geometryStorage[name];
}
