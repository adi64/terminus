#include "resourcemanager.h"

#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include "geometry.h"

#include <QDebug> //TODO remove in the end

namespace terminus
{

ResourceManager* ResourceManager::m_instance = nullptr;

ResourceManager* ResourceManager::getInstance()
{
    if(m_instance == nullptr)
    {
        m_instance = new ResourceManager();
    }

    return m_instance;
}

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
    //TODO delete m_geometryStorage contents
}

void ResourceManager::storeGeometry(std::string name, std::string path)
{
    Geometry geometry = loadGeometry(path);
    setGeometry(name, &geometry);
}

Geometry ResourceManager::loadGeometry(std::string path)
{

}



void ResourceManager::loadMaterial(std::string path)
{
    //TODO: parse the .mtl file
}

void ResourceManager::setGeometry(std::string name, Geometry * geometry)
{
    if(m_geometryStorage.count(name) == 0)
    {
        m_geometryStorage[name] = std::shared_ptr<std::unique_ptr<Geometry>>(new std::unique_ptr<Geometry>(geometry));
    }
    else
    {
        qDebug() << "Geometry was already stored";
    }
}

std::shared_ptr<std::unique_ptr<Geometry>> ResourceManager::getGeometry(std::string name)
{
    if(m_geometryStorage.count(name) == 0)
        m_geometryStorage[name] = std::shared_ptr<std::unique_ptr<Geometry>>(new std::unique_ptr<Geometry>(new Geometry()));
    return m_geometryStorage[name];
}

} // terminus
